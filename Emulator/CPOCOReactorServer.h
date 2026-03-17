#pragma once
#include <thread>
#include <memory>
#include <functional>

class CPOCOServerAcceptor;

namespace Poco {
	namespace Net {
		class StreamSocket;
		class SocketReactor;
		class ServerSocket;
	}
}

////////////////////////////////////////////////////////////////////////////////////////
/// class CPOCOServerHandler

class CPOCOServerHandler
{
public:
	CPOCOServerHandler(Poco::Net::StreamSocket& socket);
	~CPOCOServerHandler();

	Poco::Net::StreamSocket& socket()
	{
		return m_socket;
	}

	virtual void onConnect() = 0;
	virtual void onDisconnect() = 0;	
	virtual void onReadable(byte* buffer, int length) = 0;
	virtual void onShutdown() = 0;
	virtual void onError() = 0;	

private:
	Poco::Net::StreamSocket& m_socket;
};

class CPOCOServerEchoHandler : public CPOCOServerHandler
{
public:
	CPOCOServerEchoHandler(Poco::Net::StreamSocket& socket);
	~CPOCOServerEchoHandler();

	virtual void onConnect();
	virtual void onDisconnect();	
	virtual void onReadable(byte* buffer, int length);
	virtual void onShutdown();
	virtual void onError();	
};

////////////////////////////////////////////////////////////////////////////////////////
/// class CPOCOReactorServer

class CPOCOReactorServer
{
public:
	CPOCOReactorServer();
	~CPOCOReactorServer();

	bool Activate(int port);	// CPOCOEchoHandler µî·Ï
	bool Activate(int port, std::function<CPOCOServerHandler* (Poco::Net::StreamSocket&)> creator);
	bool Deactivate();

private:	
	void ThreadLoop();

private:
	int m_port;

	std::unique_ptr<Poco::Net::SocketReactor>    m_reactor;
	std::unique_ptr<Poco::Net::ServerSocket>     m_server_socket;	
	std::unique_ptr<CPOCOServerAcceptor>		 m_acceptor;
	std::function<CPOCOServerHandler* (Poco::Net::StreamSocket&)> m_creator;

	std::thread m_thread;
	std::atomic<bool> m_thread_running;
};

