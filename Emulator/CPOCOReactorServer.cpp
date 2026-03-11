#include "pch.h"
#include "CPOCOReactorServer.h"
#include "CPOCOServerHandler.h"

#include <Poco/Net/SocketReactor.h>
#include <Poco/Net/StreamSocket.h>
#include <Poco/Net/SocketNotification.h>
//#include <winsvc.h>
//#include <Poco/Util/ServerApplication.h>
#include <Poco/Net/SocketAcceptor.h>

////////////////////////////////////////////////////////////////////////////////////////
/// class ServerSession

class ServerSession
{
public:
    ServerSession(Poco::Net::StreamSocket& socket, Poco::Net::SocketReactor& reactor) :
        m_socket(socket),
        m_reactor(reactor),
        m_service(nullptr),
        m_buffer(nullptr),
        m_buffer_len(1024)
    {        
        m_buffer = new byte[m_buffer_len];

        m_peer_address = socket.peerAddress().toString();
        LOGI << fmt::format("Connection from[{}]", m_peer_address);

        //m_reactor.addEventHandler(m_socket, Poco::Observer<ServerSession, Poco::Net::WritableNotification>(*this, &ServerSession::onWritable));
        m_reactor.addEventHandler(m_socket, Poco::Observer<ServerSession, Poco::Net::ReadableNotification>(*this, &ServerSession::onReadable));
        m_reactor.addEventHandler(m_socket, Poco::Observer<ServerSession, Poco::Net::ShutdownNotification>(*this, &ServerSession::onShutdown));
        m_reactor.addEventHandler(m_socket, Poco::Observer<ServerSession, Poco::Net::ErrorNotification>(*this, &ServerSession::onError));
        //m_reactor.addEventHandler(m_socket, Poco::Observer<ServerSession, Poco::Net::IdleNotification>(*this, &ServerSession::onIdle));
        //m_reactor.addEventHandler(m_socket, Poco::Observer<ServerSession, Poco::Net::TimeoutNotification>(*this, &ServerSession::onTimeout));

    }

    ~ServerSession()
    {
        LOGI << fmt::format("Disconnection from[{}]", m_peer_address);

        //m_reactor.removeEventHandler(m_socket, Poco::Observer<ServerSession, Poco::Net::WritableNotification>(*this, &ServerSession::onWritable));
        m_reactor.removeEventHandler(m_socket, Poco::Observer<ServerSession, Poco::Net::ReadableNotification>(*this, &ServerSession::onReadable));
        m_reactor.removeEventHandler(m_socket, Poco::Observer<ServerSession, Poco::Net::ShutdownNotification>(*this, &ServerSession::onShutdown));
        m_reactor.removeEventHandler(m_socket, Poco::Observer<ServerSession, Poco::Net::ErrorNotification>(*this, &ServerSession::onError));
        //m_reactor.removeEventHandler(m_socket, Poco::Observer<ServerSession, Poco::Net::IdleNotification>(*this, &ServerSession::onIdle));
        //m_reactor.removeEventHandler(m_socket, Poco::Observer<ServerSession, Poco::Net::TimeoutNotification>(*this, &ServerSession::onTimeout));

        if (nullptr != m_buffer)
        {
            delete[] m_buffer;
            m_buffer = nullptr;
        }
    }

    Poco::Net::StreamSocket& socket()
    {
        return m_socket;
    }

    void close()
    {
        if (nullptr != m_service)
            m_service->onDisconnect(&m_socket);

        m_socket.close();        

        delete this;
    }

    void SetService(CPOCOServerHandler* service)
    {
        m_service = service;        
        if (m_service)
            m_service->onConnect(&m_socket);
    }

    void onWritable(Poco::Net::WritableNotification* pNf)
    {
        pNf->release();        
    }

    void onReadable(Poco::Net::ReadableNotification* pNf)
    {
        pNf->release();        
        try
        {
            int n = m_socket.receiveBytes(m_buffer, m_buffer_len);            
            if (n > 0)
            {
                if (m_service)
                    m_service->onReadable(&m_socket, m_buffer, n);
            }
            else
            {
                LOGI << fmt::format("Disconnection from[{}]", m_peer_address);
                close();
            }
        }
        catch (Poco::Exception& e)
        {
            LOGE << fmt::format("POCO Sesseion [{}] error : [{}]", m_peer_address, e.displayText());
            close();
        }
    }

    void onShutdown(Poco::Net::ShutdownNotification* pNf)
    {
        LOGI << fmt::format("onShutdown from[{}]", m_peer_address);
        pNf->release();        

        if (m_service)
            m_service->onShutdown(&m_socket);

        close();
    }

    void onError(Poco::Net::ErrorNotification* pNf)
    {
        pNf->release();
        LOGI << fmt::format("onError from[{}]", m_peer_address);

        if (m_service)
            m_service->onError(&m_socket);

        close();
    }

    void onTimeout(Poco::Net::TimeoutNotification* pNf)
    {
        pNf->release();        
    }

    void onIdle(Poco::Net::IdleNotification* pNf)
    {
        pNf->release();        
    }

private:
    Poco::Net::StreamSocket		m_socket;
    std::string					m_peer_address;
    Poco::Net::SocketReactor&   m_reactor;
    CPOCOServerHandler*         m_service;
    byte*                       m_buffer;
    const int				    m_buffer_len;
};

////////////////////////////////////////////////////////////////////////////////////////
/// class CPOCOServerAcceptor

class CPOCOServerAcceptor : public Poco::Net::SocketAcceptor<ServerSession>
{
public:
    CPOCOServerAcceptor(Poco::Net::ServerSocket& socket, Poco::Net::SocketReactor& reactor, CPOCOServerHandler* handler)
        : Poco::Net::SocketAcceptor<ServerSession>(socket, reactor)
        , m_handler(handler)
        , m_count(0)
    {
    }
    ~CPOCOServerAcceptor()
    {    
    }

    virtual ServerSession* createServiceHandler(Poco::Net::StreamSocket& socket)
    {
        ServerSession* session = new ServerSession(socket, *reactor());
        session->SetService(m_handler);
        return session;
    }

private:
    CPOCOServerHandler* m_handler;
    int                 m_count;
};

////////////////////////////////////////////////////////////////////////////////////////
/// class CPOCOReactorServer

CPOCOReactorServer::CPOCOReactorServer()
{
    m_port = 20101;
    m_thread_running = false;
}

CPOCOReactorServer::~CPOCOReactorServer()
{

}

bool CPOCOReactorServer::Activate(int port)
{    
    return Activate(port, new CPOCOServerEchoHandler());
}

bool CPOCOReactorServer::Activate(int port, CPOCOServerHandler* handler)
{
    LOGI_AFL << fmt::format("POCO Reactor Server Activate Port[{}]", port);

    if (true == m_thread_running)
        return false;

    if (nullptr == handler)
        handler = new CPOCOServerEchoHandler();

    m_port = port;
    m_handler = handler;

    if (m_thread.joinable())
        m_thread.join();

    m_thread = std::thread([this]()
    {
        m_thread_running = true;
        ThreadLoop();
        m_thread_running = false;
    });

    return true;
}

bool CPOCOReactorServer::Deactivate()
{
    LOGI_AFL << fmt::format("POCO Reactor Server Deactivate Port[{}]", m_port);

    if (m_reactor)
        m_reactor->stop();

    if (m_thread.joinable())
        m_thread.join();

    return true;
}

void CPOCOReactorServer::ThreadLoop()
{
    LOGI_AFL << fmt::format("POCO Reactor Server Thread");

    try
    {        
        m_reactor = std::make_unique<Poco::Net::SocketReactor>();
        //Poco::Timespan timeout(10, 0);
        //m_reactor->setTimeout(timeout);
        m_server_socket = std::make_unique<Poco::Net::ServerSocket>(m_port);
        m_acceptor = std::make_unique<CPOCOServerAcceptor>(*m_server_socket, *m_reactor, m_handler);

        m_reactor->run();
    }
    catch (Poco::Exception& e)
    {
        LOGE << fmt::format("POCO Reactor Server error : [{}]", e.displayText());
    }

    m_acceptor.reset();
    m_reactor.reset();
    m_server_socket.reset();
}