#ifndef HTTP_CLIENT_H
#define HTTP_CLIENT_H

#include <string>
#include <boost/asio.hpp>

namespace thing_speak {

enum class HTTPMessageType : int
{
    GET,
    POST
};


class http_client
{
    using namespace boost::asio;
public:
    http_client(std::string host_name_or_ip_addr);
    ~http_client();

    int SendMessage(HTTPMessageType, std::string message, std::string &result);

private:
    int Connect();
    int SendData();
    int ReadAnswer();
    void PrepareHeader();

private:
    const std::string m_host_name_or_ip_addr;
    ip::tcp::resolver m_resolver;
    ip::tcp::socket m_soket;
    ip::tcp::resolver::query m_query;
    io_service m_io_service;
    ip::tcp::resolver m_resolver;
};

}

#endif // HTTP_CLIENT_H
