#ifndef HTTP_CLIENT_H
#define HTTP_CLIENT_H

#include <string>
#include <iostream>
#include <sstream>
#include <array>
#include <vector>
#include <mutex>
#include <condition_variable>


#include <boost/asio.hpp>

namespace thing_speak {

enum class HTTPMessageType : int
{
    GET,
    POST
};


class http_client
{
    static const int kReadBufferSize = 1024;
public:
    http_client(std::string host_name_or_ip_addr);

    int SendMessage(const std::stringstream &message, std::vector<char> &answer);

private:
    int Connect();
    int SendData(const std::stringstream &message);
    int ReadAnswer(std::vector<char> &answer);
    void ReadAnswerAsync(std::vector<char> &answer);
    void OnRead(const boost::system::error_code& error, // Result of operation.
                                std::size_t bytes_transferred);       // Number of bytes read.
    void PrepareHeader();

private:
    const std::string m_host_name_or_ip_addr;
    boost::asio::io_service m_io_service;
    boost::asio::ip::tcp::socket m_socket;
    boost::asio::ip::tcp::resolver m_resolver;

    char m_tmp_array[kReadBufferSize];
    std::vector<char> m_message;

    std::mutex m_read_mutex;
    std::condition_variable m_read_complite;
    bool m_read_end;
};

}

#endif // HTTP_CLIENT_H
