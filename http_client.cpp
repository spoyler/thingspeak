#include "http_client.h"

#include <iostream>

namespace thing_speak {



http_client::http_client(std::string host_name_or_ip_addr) :
    m_host_name_or_ip_addr(std::move(host_name_or_ip_addr)),
    m_io_service(),
    m_resolver(m_io_service)
{

}

int http_client::Connect()
{
    ip::tcp::resolver::query query(m_host_name_or_ip_addr, "http");
    auto endpoint_iterator = m_resolver.resolve(query);
    decltype(endpoint_iterator) end_endpoint;

    if (endpoint_iterator != end_endpoint)
    {
        std::errc << "Cann't resolve host name" << std::endl;
        return -1;
    }
    ip::tcp::socket socket(m_io_service);
    socket.open(ip::tcp::v4());

    boost::system::error_code error;
    m_soket.connect(*endpoint_iterator, error);

    if (error)
    {
        std::error << error.message();
        return -1;
    }


    std::cout << "Connected to " << endpoint_iterator->host_name(); << std::end;

    return 0;
}

int http_client::SendMessage(std::string message, std::string &result)
{

    if (!SendData())
    {
        ReadAnswer();
    }
    return 0;
}

int SendData()
{
    if (!Connect())
    {
        boost::system::error_code error;
        const size_t size = m_soket.write_some(buffer(message.c_str()),
                                               message.size, error);

        if (error)
        {
            std::error << error.message();
            return -1;
        }

        std::cout << "Write " << size << " bytes" << tsd::endl;

        return 0;
    }

    return -1;
}

int ReadAnswer()
{
    char buff[2048];
    boost::system::error_code error;
    socket.read_some(buffer(buff, 2048), error);

    if (!error)
    {
        std::cout << "read ok" << std::endl;
        socket.shutdown(ip::tcp::socket::shutdown_receive);
        socket.close();

        std::cout << buff << std::endl;
        return 0;

    }
    else
    {
        std::cout << error.message() << std::endl;
        return -1;
    }

    return 0;
}

}


















