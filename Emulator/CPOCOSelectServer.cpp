#include "pch.h"
#include "CPOCOSelectServer.h"

#include <Poco/Net/ServerSocket.h>
#include <Poco/Net/StreamSocket.h>
#include <Poco/Net/SocketAddress.h>
#include <Poco/Exception.h>

//////////////////////////////////////////////////////////////////////////////////

CPOCOServer::CPOCOServer()
    : m_pile_len(0)
{
    
}

CPOCOServer::~CPOCOServer()
{

}

int CPOCOServer::Activate(const std::string& ip, int port)
{
    LOGI_AFL << fmt::format("Server activate ip[{}] port[{}]", ip, port);

    m_ip = ip;
    m_port = port;
    m_activate = true;
    m_pile_len = 0;

    SaveThreadName(fmt::format("Server[{}:{}]", ip, port));
    StartThread();
    
    return 0;
}

int CPOCOServer::Deactivate()
{
    LOGI_AFL << fmt::format("Server Deactivate ip[{}] port[{}]", m_ip, m_port);

    if (m_activate)
    {
        m_activate = false;
        JoinThread();
    }

    return 0;
}

int CPOCOServer::OnReceive(poco_socket* client, byte* recv_buffer, int recv_len, int& found_len)
{
    return 0;
}

int CPOCOServer::OnConnect(poco_socket* client)
{
    LOGI << fmt::format("New client connected...  Address[{}]", client->address().toString());

    return 0;
}

int CPOCOServer::OnDisconnect(poco_socket* client)
{
    LOGI << fmt::format("Client disconnected...  Address[{}]", client->address().toString());

    return 0;
}

int CPOCOServer::RecvBytes(poco_socket* client, byte* buffer, int len)
{
    int recv_len = 0;

    try
    {
        recv_len = client->receiveBytes(m_buffer, len);
        LOGI << fmt::format("Read data...  Address[{}], Len[{}/{}] pile_len[{}]", client->address().toString(), recv_len, len, m_pile_len);
    }
    catch (Poco::Exception& ex)
    {
        LOGE << fmt::format("POCO Exception message[{}] code[{}]", ex.displayText(), ex.code());
    }

    return recv_len;
}

int CPOCOServer::SendBytes(poco_socket* client, byte* buffer, int len)
{
    int send_len = 0;

    try
    {
        std::lock_guard<std::recursive_mutex> lock(m_mutex_clients);
        send_len = client->sendBytes(buffer, len);
    }
    catch (Poco::Exception& ex)
    {
        LOGE << fmt::format("POCO Exception message[{}] code[{}]", ex.displayText(), ex.code());
    }

    return send_len;
}

void CPOCOServer::ThreadLoop()
{
    LOGI << fmt::format("Server thread start");

    // 서버 초기화 설정
    Poco::Net::SocketAddress server_addr(m_ip, m_port);
    Poco::Net::ServerSocket server_sock(server_addr);

    // 접속된 소켓들을 관리할 리스트를 생성한다.
    Poco::Net::Socket::SocketList connectedSockList;

    // 서버 소켓을 리스트에 넣어준다.
    connectedSockList.push_back(server_sock);

    while (m_activate)
    {
        Poco::Timespan timeout(1);

        Poco::Net::Socket::SocketList readList(connectedSockList.begin(), connectedSockList.end());
        Poco::Net::Socket::SocketList writeList(connectedSockList.begin(), connectedSockList.end());
        Poco::Net::Socket::SocketList exceptList(connectedSockList.begin(), connectedSockList.end());

        auto count = Poco::Net::Socket::select(readList, writeList, exceptList, timeout);
        if (count == 0)
            continue;

        Poco::Net::Socket::SocketList delSockList;

        for (auto& readSock : readList)
        {
            // 서버 소켓이라면 연결 요청을 받은 것이므로 연결을 수락하고
            // 연결된 소켓 리스트에 추가해준다.
            if (server_sock == readSock)
            {
                poco_socket newSock = server_sock.acceptConnection();

                std::lock_guard<std::recursive_mutex> lock(m_mutex_clients);
                connectedSockList.push_back(newSock);
                OnConnect(&newSock);
            }
            else
            {
                auto readIter = std::find_if(connectedSockList.begin(), connectedSockList.end(), [&readSock](auto& sock)
                {
                    return readSock == sock ? true : false;
                });

                if (readIter == connectedSockList.end())
                    continue;

                Poco::Net::Socket& client = *readIter;
                poco_socket* client_p = (poco_socket*)&client;
                //poco_socket* client = (poco_socket*)&readSock;

                int len = RecvBytes(client_p, &m_buffer[m_pile_len], sizeof(m_buffer)- m_pile_len);
                m_pile_len += len;

                if (len > 0)
                {
                    int found_len = 0;
                    OnReceive(client_p, m_buffer, m_pile_len, found_len);
                    m_pile_len -= found_len;

                    if (m_pile_len > 0)
                        memcpy(&m_buffer[0], &m_buffer[found_len], m_pile_len);
                }
                else
                {
                    OnDisconnect(client_p);

                    delSockList.push_back(readSock);
                }
            }
        }

        for (auto& delSock : delSockList)
        {
            // 삭제할 소켓을 검색한다.
            auto delIter = std::find_if(connectedSockList.begin(), connectedSockList.end(), [&delSock](auto& sock)
            {
                return delSock == sock ? true : false;
            });

            if (delIter != connectedSockList.end())
            {
                std::lock_guard<std::recursive_mutex> lock(m_mutex_clients);
                connectedSockList.erase(delIter);
            }
        }
    }

    for (auto& delSock : connectedSockList)
        delSock.close();

    connectedSockList.clear();

    LOGI << fmt::format("Server thread end");
}