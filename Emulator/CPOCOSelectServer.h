#pragma once

#include "common/InnerThread.h"
#include <memory>
#include <mutex>

namespace Poco {
    namespace Net {
        class Socket;
        class StreamSocket;
    }
}

//////////////////////////////////////////////////////////////////////////////////

using poco_socket = Poco::Net::StreamSocket;

class CPOCOServer : public InnerThread
{
private:
    std::string     m_ip;
    int             m_port = 0;

    bool            m_activate = false;

    byte            m_buffer[1024 * 4];
    int             m_pile_len;

    std::recursive_mutex  m_mutex_clients;

private:
    virtual void ThreadLoop();

public:
    CPOCOServer();
    virtual ~CPOCOServer();

    std::string GetIpAddress()
    {
        return m_ip;
    }
    int GetPortNumber()
    {
        return m_port;
    }

    virtual int Activate(const std::string& ip, int port);
    virtual int Deactivate();

    virtual int OnConnect(poco_socket* client);
    virtual int OnDisconnect(poco_socket* client);
    virtual int OnReceive(poco_socket* client, byte* recv_buffer, int recv_len, int& found_len);

    virtual int SendBytes(poco_socket* client, byte* buffer, int len);
    virtual int RecvBytes(poco_socket* client, byte* buffer, int len);
};



