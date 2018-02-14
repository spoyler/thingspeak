#include "tcpcontext.h"


void TCPContext::Start()
{
    if (m_work)
        return;

    m_work.emplace(m_io_service);

    m_worker = std::thread([this]()
    {
        m_io_service.run();
        return 0;
    });
}

void TCPContext::Stop()
{
    if (!m_work)
        return;

    m_work = boost::none;
    m_worker.join();
}

