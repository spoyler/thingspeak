#include "http_client.h"


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
        ReadAnswer(answer);
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

}


















