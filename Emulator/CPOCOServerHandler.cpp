#include "pch.h"
#include "CPOCOServerHandler.h"

#include <Poco/Net/StreamSocket.h>

////////////////////////////////////////////////////////////////////////////////////////
/// class CPOCOEchoHandler

CPOCOServerEchoHandler::CPOCOServerEchoHandler()
{
}

CPOCOServerEchoHandler::~CPOCOServerEchoHandler()
{
}

void CPOCOServerEchoHandler::onConnect(PocoSocket* socket)
{

}

void CPOCOServerEchoHandler::onDisconnect(PocoSocket* socket)
{

}

void CPOCOServerEchoHandler::onReadable(PocoSocket* socket, byte* buffer, int length)
{
    socket->sendBytes(buffer, length);
}

void CPOCOServerEchoHandler::onShutdown(PocoSocket* socket)
{
}

void CPOCOServerEchoHandler::onError(PocoSocket* socket)
{
}
