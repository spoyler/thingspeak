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
   m_socket.open(boost::asio::ip::tcp::v4());
}

int http_client::SendMessage(const std::stringstream &message, std::vector<char> &answer)
{
    if (!Connect())
    {
        if (!SendData(message))
        {
            return ReadAnswerAsync(answer);
        }
    }
    return -1;
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

    boost::asio::connect(m_socket, endpoint_iterator, error);

    if (error)
    {
        std::cerr << error.message();
        return -1;
    }

    std::cout << "Connected to " << endpoint_iterator->host_name() << std::endl;

    return 0;
}

int http_client::SendData(const std::stringstream &message)
{
    boost::system::error_code error;
    const size_t size = m_socket.write_some(boost::asio::buffer(message.str().c_str(),
                                          message.str().size()), error);
    if (error)
    {
       std::cerr << error.message();
       return -1;
    }

    std::cout << "Writed " << size << " bytes" << std::endl;

    return 0;
}

int http_client::ReadAnswerAsync(std::vector<char> &answer)
{
    m_message.clear();

    m_socket.async_read_some(boost::asio::buffer(m_tmp_array),
                             std::bind(&http_client::OnRead, this,
                                std::placeholders::_1, std::placeholders::_2));
    m_read_end = false;

    m_io_service.run();

    answer = m_message;

    return m_read_end ? 0 : -1;
}

void http_client::OnRead(const boost::system::error_code& error, // Result of operation.
                            std::size_t bytes_transferred)       // Number of bytes read.
{
    if (!error && bytes_transferred)
    {
        m_message.resize(m_message.size() + bytes_transferred);

        memcpy(&m_message[m_message.size() - bytes_transferred], m_tmp_array, bytes_transferred);

        m_socket.async_read_some(boost::asio::buffer(m_tmp_array, kReadBufferSize),
                                 std::bind(&http_client::OnRead, this,
                                    std::placeholders::_1, std::placeholders::_2));
    }
    else
    {
        // close connection
        m_socket.shutdown(boost::asio::ip::tcp::socket::shutdown_receive);
        m_socket.close();
        m_read_end = true;
    }
}


}


















