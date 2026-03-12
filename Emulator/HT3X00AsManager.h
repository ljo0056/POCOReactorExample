#pragma once

#include "CPOCOServerHandler.h"
#include "HT3X00AS/XHt3000AsPacket.h"

#include <memory>
#include <random>
#include <list>
#include <chrono>
#include <functional>
#include <thread>
#include <mutex>
#include <map>

#include "common/Locker.h"

class XHt3000AsConfig;
class XHt3000AsInform;
class XHt3000AsSetup;
class XHt3000AsState;

class HT3X00AsManager : public CPOCOServerHandler
{
private:
    using chrono_tp = std::chrono::high_resolution_clock::time_point;
    using chrono_duration_milli = std::chrono::duration<double, std::milli>;

    HWND         m_hwnd = NULL;

    byte*        m_buffer;
    const int    m_buffer_size;
    int          m_recv_len;

    XHt3000Status1NewCode m_error = StandBy;

    std::map<XHt3000PacketCommand, std::string>     m_command_name;
    std::map<XHt3000Status1NewCode, std::string>    m_status1_new_name;
    std::map<XHt3000Status1Code, std::string>       m_status1_name;

    std::unique_ptr<XHt3000AsInform>    m_packet_inform;
    std::unique_ptr<XHt3000AsConfig>    m_packet_config;
    std::unique_ptr<XHt3000AsSetup>     m_packet_setup;
    std::unique_ptr<XHt3000AsState>     m_packet_state;

    enum WORK_ID
    {
        WORK_START = 0,   
        WORK_ACTIVATE,
        WORK_ABORT_INJECT,
        WORK_SINGLE_INJECT,        
        WORK_ERROR,
        WORK_END = 30,
    };
    std::mt19937        m_gen;

    struct WORK_INFO
    {
        int     id = 0;
        std::function<int(void)> work;
    };

    Locker               m_work_event;
    std::list<WORK_INFO> m_works;
    std::recursive_mutex m_mutex;    
    std::thread          m_thread;
    std::atomic<bool>    m_thread_run;

private:
    void AddWork(int id, const std::function<int(void)>& work);
    void AddWork(WORK_INFO& work_info);
    void ProcWork();
    void DeleteAllWork();

    std::string GetStatusNewName(BYTE status);
    std::string GetStatusNewName(XHt3000Status1NewCode status);
    std::string GetStatusName(BYTE status);
    std::string GetStatusName(XHt3000Status1Code status);

    int Sleep(int ms);
    int SetStatus(XHt3000Status1Code status1);
    int SetStatusNew(XHt3000Status1NewCode status1_new);
    int SetStatus2(XHt3000Status1Code status1, XHt3000Status1NewCode status1_new);
    int SetStatus3(XHt3000Status1Code status1, XHt3000Status1NewCode status1_new, BYTE status2);

    int Parse(PocoSocket* socket, byte* buffer, int length, int& found_len);
    void OnPacketHandler(XHt3000PacketCommand cmd, PocoSocket* socket, XHt3000AsPacket* packet, const ReadOnlySpan& slot, bool log);    
    void OnSingleRunEx2(PocoSocket* socket, const ReadOnlySpan& slot);    
    void OnAbort(PocoSocket* socket, const ReadOnlySpan& slot);

public:
    HT3X00AsManager();    
    ~HT3X00AsManager();

    int Activate(HWND hwnd);
    int Deactivate();

    int PostUpdateMessage();
    CString GetStatusStr();

    XHt3000AsInform* GetInform()
    {
        return m_packet_inform.get();
    }
    
    XHt3000AsConfig* GetConfig()
    {
        return m_packet_config.get();
    }    

    XHt3000AsSetup* GetSetup()
    {
        return m_packet_setup.get();
    }

    void SetError(XHt3000Status1NewCode status1_new);

    virtual void onConnect(PocoSocket* socket) override;
    virtual void onDisconnect(PocoSocket* socket) override;
    virtual void onReadable(PocoSocket* socket, byte* buffer, int length) override;
    virtual void onShutdown(PocoSocket* socket) override;
    virtual void onError(PocoSocket* socket) override;    
};

void HT3X00AsManager_Test1();


