#include "pch.h"
#include "CPOCOReactorClient.h"

#include <Poco/Net/SocketReactor.h>
#include <Poco/Net/StreamSocket.h>
#include <Poco/Net/SocketNotification.h>
#include <Poco/Net/SocketConnector.h>

////////////////////////////////////////////////////////////////////////////////////////
/// class ClientSession

class ClientSession
{
public:
	ClientSession(Poco::Net::StreamSocket& socket, Poco::Net::SocketReactor& reactor) :
		m_socket(socket),
		m_reactor(reactor),
		m_buffer_len(1024)
	{
		m_service = nullptr;
		m_buffer = new byte[m_buffer_len];

		m_peer_address = socket.peerAddress().toString();
		LOGI << fmt::format("Connection from[{}]", m_peer_address);

		//m_reactor.addEventHandler(m_socket, Poco::Observer<ClientSession, Poco::Net::WritableNotification>(*this, &ClientSession::onWritable));
		m_reactor.addEventHandler(m_socket, Poco::Observer<ClientSession, Poco::Net::ReadableNotification>(*this, &ClientSession::onReadable));
		m_reactor.addEventHandler(m_socket, Poco::Observer<ClientSession, Poco::Net::ShutdownNotification>(*this, &ClientSession::onShutdown));
		m_reactor.addEventHandler(m_socket, Poco::Observer<ClientSession, Poco::Net::ErrorNotification>(*this, &ClientSession::onError));
		//m_reactor.addEventHandler(m_socket, Poco::Observer<ClientSession, Poco::Net::IdleNotification>(*this, &ClientSession::onIdle));
		//m_reactor.addEventHandler(m_socket, Poco::Observer<ClientSession, Poco::Net::TimeoutNotification>(*this, &ClientSession::onTimeout));		
	}

	~ClientSession()
	{
		LOGI << fmt::format("Disconnection from[{}]", m_peer_address);

		//m_reactor.removeEventHandler(m_socket, Poco::Observer<ClientSession, Poco::Net::WritableNotification>(*this, &ClientSession::onWritable));
		m_reactor.removeEventHandler(m_socket, Poco::Observer<ClientSession, Poco::Net::ReadableNotification>(*this, &ClientSession::onReadable));
		m_reactor.removeEventHandler(m_socket, Poco::Observer<ClientSession, Poco::Net::ShutdownNotification>(*this, &ClientSession::onShutdown));
		m_reactor.removeEventHandler(m_socket, Poco::Observer<ClientSession, Poco::Net::ErrorNotification>(*this, &ClientSession::onError));
		//m_reactor.removeEventHandler(m_socket, Poco::Observer<ClientSession, Poco::Net::IdleNotification>(*this, &ClientSession::onIdle));
		//m_reactor.removeEventHandler(m_socket, Poco::Observer<ClientSession, Poco::Net::TimeoutNotification>(*this, &ClientSession::onTimeout));		

		if (nullptr != m_service)
		{
			delete m_service;
			m_service = nullptr;
		}

		if (nullptr != m_buffer)
		{
			delete[] m_buffer;
			m_buffer = nullptr;
		}		
	}

	void close()
	{
		m_socket.close();

		if (nullptr != m_service)
			m_service->onDisconnect();

		m_reactor.stop();

		delete this;
	}

	void SetService(CPOCOClientHandler* service)
	{
        m_service = service;

		if (m_service)
			m_service->onConnect();
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
					m_service->onReadable(m_buffer, n);				
			}
			else
			{
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
		pNf->release();
		LOGI << fmt::format("onShutdown from[{}]", m_peer_address);

		if (m_service)
			m_service->onShutdown();
		
		close();
	}

	void onError(Poco::Net::ErrorNotification* pNf)
	{
		pNf->release();
		LOGI << fmt::format("onError from[{}]", m_peer_address);

		if (m_service)
			m_service->onError();

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
	Poco::Net::SocketReactor&	m_reactor;
	CPOCOClientHandler*			m_service;
	byte* m_buffer;
	const int				    m_buffer_len;
};


////////////////////////////////////////////////////////////////////////////////////////
/// class CPOCOClientHandler

CPOCOClientHandler::CPOCOClientHandler(Poco::Net::StreamSocket& socket)
	: m_socket(socket)
{
};
CPOCOClientHandler::~CPOCOClientHandler()
{
};

////////////////////////////////////////////////////////////////////////////////////////
/// class CPOCOClientEchoHandler

CPOCOClientEchoHandler::CPOCOClientEchoHandler(Poco::Net::StreamSocket& socket)
	: CPOCOClientHandler(socket)
{
}

CPOCOClientEchoHandler::~CPOCOClientEchoHandler()
{
}

void CPOCOClientEchoHandler::onConnect()
{
}

void CPOCOClientEchoHandler::onDisconnect()
{
}

void CPOCOClientEchoHandler::onReadable(byte* buffer, int length)
{
	socket().sendBytes(buffer, length);
}

void CPOCOClientEchoHandler::onShutdown()
{
}

void CPOCOClientEchoHandler::onError()
{
}

////////////////////////////////////////////////////////////////////////////////////////
/// class CPOCOClientConnector

class CPOCOClientConnector : public Poco::Net::SocketConnector<ClientSession>
{
public:
	CPOCOClientConnector(
		Poco::Net::SocketAddress& address, Poco::Net::SocketReactor& reactor,
		std::function<CPOCOClientHandler* (Poco::Net::StreamSocket&)> creator)
		: Poco::Net::SocketConnector<ClientSession>(address, reactor)
		, m_creator(creator)
	{
	}

	virtual ClientSession* createServiceHandler()
	{
		CPOCOClientHandler* service = m_creator(socket());
		ClientSession* session = new ClientSession(socket(), *reactor());
		session->SetService(service);

		return session;
	}

	virtual void onError(int errorCode)
	{
		LOGI << fmt::format("Error[{}]", errorCode);

		reactor()->stop();
	}

public:
	std::function<CPOCOClientHandler* (Poco::Net::StreamSocket&)> m_creator;
};

////////////////////////////////////////////////////////////////////////////////////////
/// class CPOCOReactorClient

CPOCOReactorClient::CPOCOReactorClient()
{
	m_thread_running = false;
}

CPOCOReactorClient::~CPOCOReactorClient()
{

}

bool CPOCOReactorClient::Activate(const std::string& ip_address, int port)
{
	std::function<CPOCOClientHandler* (Poco::Net::StreamSocket&)> echo_session = [](Poco::Net::StreamSocket& socket)
	{
		return new CPOCOClientEchoHandler(socket);
	};

	return Activate(ip_address, port, echo_session);
}

bool CPOCOReactorClient::Activate(const std::string& ip_address, int port,
	std::function<CPOCOClientHandler* (Poco::Net::StreamSocket&)> creator)
{
	LOGI << fmt::format("POCO Reactor Server Activate Port[{}]", port);

	if (true == m_thread_running)
		return false;

	m_ip_address = ip_address;
	m_port = port;	
	m_creator = creator;

	//if (m_thread.joinable())
	//	m_thread.join();
	
	m_thread = std::thread([this]()
		{
			m_thread_running = true;
			ThreadLoop();
			m_thread_running = false;
		});

	return true;
}

bool CPOCOReactorClient::Deactivate()
{
	LOGI << fmt::format("POCO Reactor Server Deactivate Port[{}]", m_port);

	if (m_reactor)
		m_reactor->stop();
	
    if (m_thread.joinable())
        m_thread.join();

	return true;
}

void CPOCOReactorClient::ThreadLoop()
{
	LOGI << fmt::format("POCO Reactor Server Thread Start");

	try
	{
		Poco::Net::SocketAddress sa(m_ip_address, m_port);
		Poco::Timespan timeout(10, 0);
		m_reactor = std::make_unique<Poco::Net::SocketReactor>(timeout);
		m_connector = std::make_unique<CPOCOClientConnector>(sa, *m_reactor, m_creator);

		m_reactor->run();		
	}
	catch (Poco::Exception& e)
	{
		LOGE << fmt::format("POCO Reactor Server error : [{}]", e.displayText());
	}

	m_connector.reset();
	m_reactor.reset();	

	LOGI << fmt::format("POCO Reactor Server Thread End");
}