#pragma once

namespace Poco {
	namespace Net {
		class StreamSocket;		
	}
}

typedef Poco::Net::StreamSocket PocoSocket;

////////////////////////////////////////////////////////////////////////////////////////
/// class CPOCOServerHandler

class CPOCOServerHandler
{
public:
	CPOCOServerHandler()
	{
	}
	~CPOCOServerHandler()
	{
	}

	virtual void onConnect(PocoSocket* socket) = 0;
	virtual void onDisconnect(PocoSocket* socket) = 0;
	virtual void onReadable(PocoSocket* socket, byte* buffer, int length) = 0;
	virtual void onShutdown(PocoSocket* socket) = 0;
	virtual void onError(PocoSocket* socket) = 0;
};

class CPOCOServerEchoHandler : public CPOCOServerHandler
{
public:
	CPOCOServerEchoHandler();
	~CPOCOServerEchoHandler();

	virtual void onConnect(PocoSocket* socket);
	virtual void onDisconnect(PocoSocket* socket);
	virtual void onReadable(PocoSocket* socket, byte* buffer, int length);
	virtual void onShutdown(PocoSocket* socket);
	virtual void onError(PocoSocket* socket);
};
