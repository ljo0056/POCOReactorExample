#pragma once
#include <thread>
#include <memory>
#include <functional>

class CPOCOClientConnector;

namespace Poco {
	namespace Net {
		class StreamSocket;
		class SocketReactor;
		class ServerSocket;
	}
}

////////////////////////////////////////////////////////////////////////////////////////
/// class CPOCOServerHandler

class CPOCOClientHandler
{
public:
    CPOCOClientHandler(Poco::Net::StreamSocket& socket);
	~CPOCOClientHandler();

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

class CPOCOClientEchoHandler : public CPOCOClientHandler
{
public:
	CPOCOClientEchoHandler(Poco::Net::StreamSocket& socket);
	~CPOCOClientEchoHandler();

	virtual void onConnect();
	virtual void onDisconnect();	
	virtual void onReadable(byte* buffer, int length);
	virtual void onShutdown();
	virtual void onError();	
};


////////////////////////////////////////////////////////////////////////////////////////
/// class CPOCOReactorClient

class CPOCOReactorClient
{
public:
	CPOCOReactorClient();
	~CPOCOReactorClient();

	bool Activate(const std::string& ip_address, int port);
	bool Activate(const std::string& ip_address, int port, std::function<CPOCOClientHandler* (Poco::Net::StreamSocket&)> creator);
	bool Deactivate();
	bool IsActivate()
	{
		return m_thread_running;
	}

private:
	void ThreadLoop();

private:
	std::string m_ip_address;
	int m_port;

	std::unique_ptr<Poco::Net::SocketReactor>    m_reactor;	
	std::unique_ptr<CPOCOClientConnector>		 m_connector;
	std::function<CPOCOClientHandler* (Poco::Net::StreamSocket&)> m_creator;

	std::thread m_thread;
	std::atomic<bool> m_thread_running;
};

