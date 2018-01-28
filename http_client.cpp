#include "http_client.h"

#include <memory>
#include <functional>


namespace thing_speak {



http_client::http_client(std::string host_name_or_ip_addr) :
    m_host_name_or_ip_addr(std::move(host_name_or_ip_addr)),
    m_io_service(),
    m_socket(m_io_service),
    m_resolver(m_io_service)
{

}

int http_client::Connect()
{
    boost::asio::ip::tcp::resolver::query query(m_host_name_or_ip_addr, "http");
    boost::system::error_code error;
    auto endpoint_iterator = m_resolver.resolve(query,error);
    decltype(endpoint_iterator) end_endpoint;

    if (error)
    {
        std::cerr << error.message();
        std::cerr << "Cann't resolve host name: " << m_host_name_or_ip_addr << std::endl;
        return -1;
    }

    boost::asio::ip::tcp::socket socket(m_io_service);
    socket.open(boost::asio::ip::tcp::v4());

    m_socket.connect(*endpoint_iterator, error);

    if (error)
    {
        std::cerr << error.message();
        return -1;
    }


    std::cout << "Connected to " << endpoint_iterator->host_name() << std::endl;

    return 0;
}

int http_client::SendMessage(const std::stringstream &message, std::vector<char> &answer)
{

    if (!SendData(message))
    {
        ReadAnswerAsync(answer);
        return 0;
    }
    return -1;
}

int http_client::SendData(const std::stringstream &message)
{
    if (!Connect())
    {
        boost::system::error_code error;
        const size_t size = m_socket.write_some(boost::asio::buffer(message.str().c_str(),
                                               message.str().size()), error);

        if (error)
        {
            std::cerr << error.message();
            return -1;
        }

        std::cout << "Write " << size << " bytes" << std::endl;

        return 0;
    }

    return -1;
}


int http_client::ReadAnswer(std::vector<char> &answer)
{
    const auto data_to_read = m_socket.available();

    if (data_to_read)
    {
        answer.resize(data_to_read);
        boost::system::error_code error;

        m_socket.read_some(boost::asio::buffer((answer), answer.size()), error);

        if (!error)
        {
            std::cout << "read ok" << std::endl;
            m_socket.shutdown(boost::asio::ip::tcp::socket::shutdown_receive);
            m_socket.close();

            return 0;
        }
        else
        {
            std::cout << error.message() << std::endl;
            return -1;
        }
    }

    return 0;
}

void http_client::ReadAnswerAsync(std::vector<char> &answer)
{
    m_message.clear();

    m_socket.async_read_some(boost::asio::buffer(m_tmp_array),
                             std::bind(&http_client::OnRead, this,
                                std::placeholders::_1, std::placeholders::_2 ));
    std::unique_lock<std::mutex> lock(m_read_mutex);

    m_read_end = false;

    while(!m_read_end)
        m_read_complite.wait(lock);

}

void http_client::OnRead(const boost::system::error_code& error, // Result of operation.
                            std::size_t bytes_transferred)       // Number of bytes read.
{
    if (!error && bytes_transferred)
    {
        m_message.resize(m_message.size() + bytes_transferred);

        memcpy(&m_message[m_message.size() - 1 - bytes_transferred], m_tmp_array, bytes_transferred);

        m_socket.async_read_some(boost::asio::buffer(m_tmp_array),
                                 std::bind(&http_client::OnRead, this,
                                    std::placeholders::_1, std::placeholders::_2 ));
    }
    else
    {
        // print result
        for (size_t i = 0; i < m_message.size(); ++i)
        {
            std::cout << m_message[i];
        }
        std::cout << std::endl;
        m_read_end = true;
        m_read_complite.notify_one();
    }
}


}


















