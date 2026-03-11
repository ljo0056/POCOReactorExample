#pragma once
#include <thread>
#include <memory>

class CPOCOServerAcceptor;
class CPOCOServerHandler;

namespace Poco {
	namespace Net {
		class StreamSocket;
		class SocketReactor;
		class ServerSocket;
	}
}

typedef Poco::Net::StreamSocket PocoSocket;

////////////////////////////////////////////////////////////////////////////////////////
/// class CPOCOReactorServer

class CPOCOReactorServer
{
public:
	CPOCOReactorServer();
	~CPOCOReactorServer();

	bool Activate(int port);	// CPOCOEchoHandler 등록
	bool Activate(int port, CPOCOServerHandler* handler);
	bool Deactivate();

private:	
	void ThreadLoop();

private:
	int m_port;

	std::unique_ptr<Poco::Net::SocketReactor>    m_reactor;
	std::unique_ptr<Poco::Net::ServerSocket>     m_server_socket;	
	std::unique_ptr<CPOCOServerAcceptor>		 m_acceptor;
	CPOCOServerHandler*                          m_handler;

	std::thread m_thread;
	std::atomic<bool> m_thread_running;
};

