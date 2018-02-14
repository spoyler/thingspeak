#ifndef TCPCONTEXT_H
#define TCPCONTEXT_H


#include <thread>

#include <boost/asio.hpp>
#include <boost/optional.hpp>

class TCPContext
{
public:
    boost::asio::io_service &IOService() {return m_io_service;}

    void Start();
    void Stop();

private:
    boost::asio::io_service m_io_service;
    boost::optional<boost::asio::io_service::work> m_work;

    std::thread m_worker;
};

#endif // TCPCONTEXT_H
