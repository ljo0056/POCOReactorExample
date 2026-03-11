#pragma once

#include <string>
#include <map>

enum XHt3000Model
{
    XHtHT3000A = 30,
    XHtHT3100A = 31,
    XHtHT3200A = 32,    
};

enum XHt3000PacketCommand {
    XHt3000CmdNone = 0x00,
	XHt3000CmdReadSampler = 0x01,
	XHt3000CmdEnableKeypad = 0x02,
	XHt3000CmdReadInformation = 0x03,
	XHt3000CmdReadSamplerEx = 0x05,
	XHt3000CmdSetParameters = 0x07,
	XHt3000CmdAutomaticRun = 0x08,
	XHt3000CmdSingleRunAtReady = 0x09,
	XHt3000CmdAbort = 0x0a,
	XHt3000CmdWashSyringe = 0x0b,
	XHt3000CmdReadSetup = 0x11,
	XHt3000CmdReadMethod = 0x12,
	XHt3000CmdWriteMethod = 0x13,
	XHt3000CmdReadSequence = 0x14,
	XHt3000CmdWriteSequence = 0x15,
	XHt3000CmdReadSequenceEx = 0x16,
	XHt3000CmdWriteSequenceEx = 0x17,
	XHt3000CmdReadSetupEx = 0x18,
	XHt3000CmdSetSetupEx = 0x19,
	XHt3000CmdSetSevice = 0x1a,
	XHt3000CmdReadEthernet = 0x1b,
	XHt3000CmdSetEthernet = 0x1c,
	XHt3000CmdReadSamplerEx2 = 0x1d,
	XHt3000CmdReadMethodEx = 0x1e,
	XHt3000CmdWriteMethodEx = 0x1f,
	XHt3000CmdReadSequenceEx2 = 0x20,
	XHt3000CmdWriteSequenceEx2 = 0x21,
	XHt3000CmdSingleRunEx = 0x22,
	XHt3000CmdAutomaticRunEx = 0x23,
	XHt3000CmdTerminateAfterThisRun = 0x24,
	XHt3000CmdMoveTray = 0x25,
	XHt3000CmdReadWashParameters = 0x27,
	XHt3000CmdWriteWashParameters = 0x28,
	XHt3000CmdReadBCR = 0x2a,
	XHt3000CmdSetBCR = 0x2b,
	XHt3000CmdReadRFID = 0x2c,
	XHt3000CmdSetRFID = 0x2d,
	XHt3000CmdReadLastBCR = 0x30,
	XHt3000CmdReadSetupEx2 = 0x32,
	XHt3000CmdSetSetupEx2 = 0x33,
	XHt3000CmdSingleRunEx2 = 0x34,
	XHt3000CmdReadSerial = 0xff,
};

enum XHt3000InjectSync
{
	XInjectSyncNormal = 0,
	XInjectSyncTrigger,
	XInjectSyncAStart,
	XInjectSyncDelay,
	XInjectSyncEA,
	XInjectSyncNormalWithoutReady,
	XInjectSyncNormalWithDataSystem,
	XInjectSyncDStartFlush = 9,
};

enum XHt3000InjectMode
{
	XInjectModeFront = 0,
	XInjectModeRear,
	XInjectModeConfirmation,
	XInjectModeHighThroughput
};

enum XHt3000Error1Code
{
    XError1None = 0,
    XErrorChecksum = (1 << 0),
    XErrorLength = (1 << 1),
    XErrorNonExcutable = (1 << 2),
    XErrorInvalidCommand = (1 << 3),
    XErrorInvalidData = (1 << 4)
};

enum XHt3000Error2Code
{
	XError2None = 0,
	XErrorSyringeMisMatch = (1 << 0),
	//XErrorTrayMisMatch = (1 << 8),
	//XErrorSolventMisMatch = (1 << 9),
	XErrorSequenceInjPort = (1 << 14)
};

enum XHt3000Status1Code
{
    // Normal status
    XStatus1StandBy = 0,
    XStatus1Running,
    XStatus1WaitGC,
    XStatus1Checking,
    XStatus1Trigger,
    XStatus1PrepFront,
    XStatus1PrepRear,
    XStatus1SyringeWashing,
    XStatus1ErrorFind,
    XStatus1AbortState,
    XStatus1SetupRun,
    XStatus1PlateCheck,
    // Extended status
    XStatus1PreWashRun,
    XStatus1SampleWashRun,
    XStatus1BubbleRemove,
    XStatus1Ready2Inject,
    XStatus1InjectFront,
    XStatus1InjectRear,
    XStatus1InjectDone,
    XStatus1PostWashRun
};

enum XHt3000Status1NewCode
{
    StandBy = 0,
    Init,
    BMOReady,
    BMOManual,
    BMORun,
    BMOAbort,
    SetupReady,
    SetupAlignFront,
    SetupAbortAlignFront,
    SetupAlignRear,
    SetupAbortAlignRear,
    SetupTouchWaste,
    SetupAbortTouchWaste,
    SetupTouchTrayVial,
    SetupAbortTrayTouch,
    SetupTouchFront,
    SetupAbortTouchFront,
    SetupTouchRear,
    SetupAbortTouchRear,
    SetupPlungerZeroRun,
    SetupAbortPlungerZero,
    SetupManual,
    StandbyTrayMove,
    StandbyAbortTrayMove,
    SyringeWashRun,
    SyringeAbortWash,
    SingleCheckParameter,
    SingleEmptyPosition,
    SingleSyncDelay,
    SinglePreWash,
    SingleSampleWash,
    SingleBubbleRemove,
    SingleDrawISTD,
    SingleDrawSample,
    SingleWait4Ready,
    SingleWaitAnalysisTime,
    SingleEmitTrigger,
    SingleWait4Trigger,
    SingleInjectFront,
    SingleInjectRear,
    SingleEmitSampleInstant,
    SinglePostWash,
    SingleReturnStandby,
    SingleAbortInject,
    SingleErrorFound,
    AutoCheckParameter,
    AutoEmptyPosition,
    AutoSyncDelay,
    AutoPreWash,
    AutoSampleWash,
    AutoBubbleRemove,
    AutoDrawISTD,
    AutoDrawSample,
    AutoWait4Ready,
    AutoWaitAnalysisTime,
    AutoEmitTrigger,
    AutoWait4Trigger,
    AutoInjectFront,
    AutoInjectRear,
    AutoEmitSampleInstant,
    AutoPostWash,
    AutoReturnStandby,
    AutoAbortInject,
    AutoErrorFound,
    SetupTestBCR,
    CheckRFID,
    SingleReadBCR,
    AutoReadBCR,
    SingleReadRFID,
    AutoReadRFID,
    Wait4Injection,
    SingleLoadingVial,
    SingleUnloadingVial,
    VialConditioning,
    WaitVialConditioning,
    SingleOvenHardwareError,
    SingleOvenTemperTimeout,
    AutoLoadingVial,
    AutoUnloadingVial,
    AutoVialConditioning,
    AutoWaitVialConditioning,
    AutoOvenHardwareError,
    AutoOvenTemperTimeout,
    SetupOvenTouch,
    SetupAbortingOvenTouch,
    SingleParameterCheck,
    SingleMoveSample,
    SingleIntermediateWash1,
    SingleAddReagent,
    SingleMixCycles,
    SingleIntermediateWash2,
    SinglewaitReaction,
    SingleReturningInStandBy,
    SingleAbortingTask,
    SingleErrorFoundDuringExecution,
    SingleCheckingParameterError1,
    MoveOverLoadingPosition,
    MoveOverStandByPosition,
    MoveOverWastePosition,
    StirrerTestInExecution,
    SingleCheckingParameterError2,
    AutoCheckingParameterError,
    SingleErrorOnConditioningTemper,
    AutoErrorOnConditioningTemper,
    RunningSeptaTestFunction,
    AbortingSeptaTestFunction,
    MovingTurretLeftOrRight,
    MovingTurretStandBy,
    MovingTurretAbort,

    SingleMissingVial = 999, // 임의로 추가함 (프로토콜 문서에는 없음)
};

enum XHt3000Status2Code
{
    XStatus2None = 0,
    XStatus2GcReady = (1 << 0),
    XStatus2GcSyncIn = (1 << 1),
    XStatus2GcAuxiliary = (1 << 2),
    XStatus2KeyLocked = (1 << 3),
    
    XStatus2VialMask = (3 << 4),
    XStatus2VialVerified = 0, // 0xCF == 1100 1111
    XStatus2VialFound = (1 << 4),
    XStatus2VialNotFound = (1 << 5),

    XStatus2InjectError = (1 << 6),
    XStatus2BarcodeRead = (1 << 7)
};

//////////////////////////////////////////////////////////////////

class ReadOnlySpan
{
private:
    const byte* m_data;
    int m_length;

public:
    ReadOnlySpan(const byte* data, int length);

    void Set(const byte* data, int length)
    {
        m_data = data;
        m_length = length;
    }
    const byte* GetData() const
    {
        return m_data; 
    }
    int GetLength() const 
    { 
        return m_length;
    }

    BYTE GetByte(int pos, int length = 2) const;
    USHORT GetUShort(int pos, int length = 4) const;
    UINT GetUInt32(int pos, int length = 8) const;
    INT GetInt32(int pos, int length = 8) const;
    void GetString(int start, int length, char* str, int str_size) const;
};

//////////////////////////////////////////////////////////////////

class XHt3000AsPacket
{
protected:
    friend class XHt3000AsPacketExtention;

	XHt3000PacketCommand cmd;
	byte error[2];
    byte status[2];
    USHORT length; // STX ... ETX 를 제외한 Packet length
	byte checksum;

public:
    BYTE error1 = 0;					// XHt3000Error1Code
    BYTE error2 = 0;
    BYTE status1 = XStatus1StandBy;		// XHt3000Status1Code	
    BYTE status2 = 0;					// XHt3000Status2Code
    BYTE status1_new = Init;			// XHt3000Status1NewCode

public:
    XHt3000AsPacket();

    XHt3000PacketCommand GetCommand() { return cmd; }
    BYTE* GetError() { return error; }
    BYTE* GetStatus() { return status; }
    USHORT GetLength() { return length; }
    BYTE GetChecksum() { return checksum; }	

    // slot 데이터를 분석하여 Packet의 맴버 변수에 저장
	// slot : LENGTH + DATA	
    virtual int Parse(XHt3000PacketCommand code, const ReadOnlySpan& slot) = 0;

    // Packet의 맴버 변수를 이용하여 slot 데이터를 생성 
    // slot : LENGTH + DATA	
    virtual int Assemble(XHt3000PacketCommand code, std::string& slot) = 0;

    virtual void PrintLog() {}
};

//////////////////////////////////////////////////////////////////

class XHt3000AsPacketExtention
{
public:
	const static byte STX = '$';
	const static byte ETX = '\r';
    const static int  PACKET_MIN_LEN = 18;  // STX + ETX 포함한 Packet 최소 길이

public:	
    static std::string GetHex(const char* str, int str_len);

    // "OPERAND + ERROR + STATUS + LENGTH + DATA" 부분에 대한 Checksum 계산을 해야함
    static byte CalcChecksum(byte* data, int length);
    static byte CalcChecksum(const std::string& data);

    static BYTE OperatorCode(const ReadOnlySpan& slot);
    static USHORT PacketLength(const ReadOnlySpan& slot);    

    // PerformParse : slot 데이터를 분석하여 Packet의 맴버 변수에 저장
	// packet : STX ... ETX 를 포함한 Packet
	// packet_len : STX ... ETX 를 포함한 Packet length
	static int PerformParse(XHt3000AsPacket* xht, const ReadOnlySpan& slot);

    // PerformAssemble : xht의 맴버 변수를 이용하여 packet 매개변수에 데이터를 생성 (STX ... ETX 포함)
    static int PerformAssemble(XHt3000AsPacket* xht, XHt3000PacketCommand code, std::string& packet);

	static int RequestPacket(XHt3000PacketCommand code, std::string& packet);

    static std::map<XHt3000PacketCommand, std::string> CommandNameMap()
    {
        std::map<XHt3000PacketCommand, std::string> m;
        m[XHt3000CmdNone] = "XHt3000CmdNone";
        m[XHt3000CmdReadSampler] = "XHt3000CmdReadSampler";
        m[XHt3000CmdEnableKeypad] = "XHt3000CmdEnableKeypad";
        m[XHt3000CmdReadInformation] = "XHt3000CmdReadInformation";
        m[XHt3000CmdReadSamplerEx] = "XHt3000CmdReadSamplerEx";
        m[XHt3000CmdSetParameters] = "XHt3000CmdSetParameters";
        m[XHt3000CmdAutomaticRun] = "XHt3000CmdAutomaticRun";
        m[XHt3000CmdSingleRunAtReady] = "XHt3000CmdSingleRunAtReady";
        m[XHt3000CmdAbort] = "XHt3000CmdAbort";
        m[XHt3000CmdWashSyringe] = "XHt3000CmdWashSyringe";
        m[XHt3000CmdReadSetup] = "XHt3000CmdReadSetup";
        m[XHt3000CmdReadMethod] = "XHt3000CmdReadMethod";
        m[XHt3000CmdWriteMethod] = "XHt3000CmdWriteMethod";
        m[XHt3000CmdReadSequence] = "XHt3000CmdReadSequence";
        m[XHt3000CmdWriteSequence] = "XHt3000CmdWriteSequence";
        m[XHt3000CmdReadSequenceEx] = "XHt3000CmdReadSequenceEx";
        m[XHt3000CmdWriteSequenceEx] = "XHt3000CmdWriteSequenceEx";
        m[XHt3000CmdReadSetupEx] = "XHt3000CmdReadSetupEx";
        m[XHt3000CmdSetSetupEx] = "XHt3000CmdSetSetupEx";
        m[XHt3000CmdSetSevice] = "XHt3000CmdSetSevice";
        m[XHt3000CmdReadEthernet] = "XHt3000CmdReadEthernet";
        m[XHt3000CmdSetEthernet] = "XHt3000CmdSetEthernet";
        m[XHt3000CmdReadSamplerEx2] = "XHt3000CmdReadSamplerEx2";
        m[XHt3000CmdReadMethodEx] = "XHt3000CmdReadMethodEx";
        m[XHt3000CmdWriteMethodEx] = "XHt3000CmdWriteMethodEx";
        m[XHt3000CmdReadSequenceEx2] = "XHt3000CmdReadSequenceEx2";
        m[XHt3000CmdWriteSequenceEx2] = "XHt3000CmdWriteSequenceEx2";
        m[XHt3000CmdSingleRunEx] = "XHt3000CmdSingleRunEx";
        m[XHt3000CmdAutomaticRunEx] = "XHt3000CmdAutomaticRunEx";
        m[XHt3000CmdTerminateAfterThisRun] = "XHt3000CmdTerminateAfterThisRun";
        m[XHt3000CmdMoveTray] = "XHt3000CmdMoveTray";
        m[XHt3000CmdReadWashParameters] = "XHt3000CmdReadWashParameters";
        m[XHt3000CmdWriteWashParameters] = "XHt3000CmdWriteWashParameters";
        m[XHt3000CmdReadBCR] = "XHt3000CmdReadBCR";
        m[XHt3000CmdSetBCR] = "XHt3000CmdSetBCR";
        m[XHt3000CmdReadRFID] = "XHt3000CmdReadRFID";
        m[XHt3000CmdSetRFID] = "XHt3000CmdSetRFID";
        m[XHt3000CmdReadLastBCR] = "XHt3000CmdReadLastBCR";
        m[XHt3000CmdReadSetupEx2] = "XHt3000CmdReadSetupEx2";
        m[XHt3000CmdSetSetupEx2] = "XHt3000CmdSetSetupEx2";
        m[XHt3000CmdSingleRunEx2] = "XHt3000CmdSingleRunEx2";
        m[XHt3000CmdReadSerial] = "XHt3000CmdReadSerial";
        return m;
    }

    static std::map<XHt3000Status1NewCode, std::string> StatusNewCodeNameMap()
    {
        std::map<XHt3000Status1NewCode, std::string> m;
        m[StandBy] = "StandBy";
        m[Init] = "Init";
        m[BMOReady] = "BMOReady";
        m[BMOManual] = "BMOManual";
        m[BMORun] = "BMORun";
        m[BMOAbort] = "BMOAbort";
        m[SetupReady] = "SetupReady";
        m[SetupAlignFront] = "SetupAlignFront";
        m[SetupAbortAlignFront] = "SetupAbortAlignFront";
        m[SetupAlignRear] = "SetupAlignRear";
        m[SetupAbortAlignRear] = "SetupAbortAlignRear";
        m[SetupTouchWaste] = "SetupTouchWaste";
        m[SetupAbortTouchWaste] = "SetupAbortTouchWaste";
        m[SetupTouchTrayVial] = "SetupTouchTrayVial";
        m[SetupAbortTrayTouch] = "SetupAbortTrayTouch";
        m[SetupTouchFront] = "SetupTouchFront";
        m[SetupAbortTouchFront] = "SetupAbortTouchFront";
        m[SetupTouchRear] = "SetupTouchRear";
        m[SetupAbortTouchRear] = "SetupAbortTouchRear";
        m[SetupPlungerZeroRun] = "SetupPlungerZeroRun";
        m[SetupAbortPlungerZero] = "SetupAbortPlungerZero";
        m[SetupManual] = "SetupManual";
        m[StandbyTrayMove] = "StandbyTrayMove";
        m[StandbyAbortTrayMove] = "StandbyAbortTrayMove";
        m[SyringeWashRun] = "SyringeWashRun";
        m[SyringeAbortWash] = "SyringeAbortWash";
        m[SingleCheckParameter] = "SingleCheckParameter";
        m[SingleEmptyPosition] = "SingleEmptyPosition";
        m[SingleSyncDelay] = "SingleSyncDelay";
        m[SinglePreWash] = "SinglePreWash";
        m[SingleSampleWash] = "SingleSampleWash";
        m[SingleBubbleRemove] = "SingleBubbleRemove";
        m[SingleDrawISTD] = "SingleDrawISTD";
        m[SingleDrawSample] = "SingleDrawSample";
        m[SingleWait4Ready] = "SingleWait4Ready";
        m[SingleWaitAnalysisTime] = "SingleWaitAnalysisTime";
        m[SingleEmitTrigger] = "SingleEmitTrigger";
        m[SingleWait4Trigger] = "SingleWait4Trigger";
        m[SingleInjectFront] = "SingleInjectFront";
        m[SingleInjectRear] = "SingleInjectRear";
        m[SingleEmitSampleInstant] = "SingleEmitSampleInstant";
        m[SinglePostWash] = "SinglePostWash";
        m[SingleReturnStandby] = "SingleReturnStandby";
        m[SingleAbortInject] = "SingleAbortInject";
        m[SingleErrorFound] = "SingleErrorFound";
        m[AutoCheckParameter] = "AutoCheckParameter";
        m[AutoEmptyPosition] = "AutoEmptyPosition";
        m[AutoSyncDelay] = "AutoSyncDelay";
        m[AutoPreWash] = "AutoPreWash";
        m[AutoSampleWash] = "AutoSampleWash";
        m[AutoBubbleRemove] = "AutoBubbleRemove";
        m[AutoDrawISTD] = "AutoDrawISTD";
        m[AutoDrawSample] = "AutoDrawSample";
        m[AutoWait4Ready] = "AutoWait4Ready";
        m[AutoWaitAnalysisTime] = "AutoWaitAnalysisTime";
        m[AutoEmitTrigger] = "AutoEmitTrigger";
        m[AutoWait4Trigger] = "AutoWait4Trigger";
        m[AutoInjectFront] = "AutoInjectFront";
        m[AutoInjectRear] = "AutoInjectRear";
        m[AutoEmitSampleInstant] = "AutoEmitSampleInstant";
        m[AutoPostWash] = "AutoPostWash";
        m[AutoReturnStandby] = "AutoReturnStandby";
        m[AutoAbortInject] = "AutoAbortInject";
        m[AutoErrorFound] = "AutoErrorFound";
        m[SetupTestBCR] = "SetupTestBCR";
        m[CheckRFID] = "CheckRFID";
        m[SingleReadBCR] = "SingleReadBCR";
        m[AutoReadBCR] = "AutoReadBCR";
        m[SingleReadRFID] = "SingleReadRFID";
        m[AutoReadRFID] = "AutoReadRFID";
        m[Wait4Injection] = "Wait4Injection";
        m[SingleLoadingVial] = "SingleLoadingVial";
        m[SingleUnloadingVial] = "SingleUnloadingVial";
        m[VialConditioning] = "VialConditioning";
        m[WaitVialConditioning] = "WaitVialConditioning";
        m[SingleOvenHardwareError] = "SingleOvenHardwareError";
        m[SingleOvenTemperTimeout] = "SingleOvenTemperTimeout";
        m[AutoLoadingVial] = "AutoLoadingVial";
        m[AutoUnloadingVial] = "AutoUnloadingVial";
        m[AutoVialConditioning] = "AutoVialConditioning";
        m[AutoWaitVialConditioning] = "AutoWaitVialConditioning";
        m[AutoOvenHardwareError] = "AutoOvenHardwareError";
        m[AutoOvenTemperTimeout] = "AutoOvenTemperTimeout";
        m[SetupOvenTouch] = "SetupOvenTouch";
        m[SetupAbortingOvenTouch] = "SetupAbortingOvenTouch";
        m[SingleParameterCheck] = "SingleParameterCheck";
        m[SingleMoveSample] = "SingleMoveSample";
        m[SingleIntermediateWash1] = "SingleIntermediateWash1";
        m[SingleAddReagent] = "SingleAddReagent";
        m[SingleMixCycles] = "SingleMixCycles";
        m[SingleIntermediateWash2] = "SingleIntermediateWash2";
        m[SinglewaitReaction] = "SinglewaitReaction";
        m[SingleReturningInStandBy] = "SingleReturningInStandBy";
        m[SingleAbortingTask] = "SingleAbortingTask";
        m[SingleErrorFoundDuringExecution] = "SingleErrorFoundDuringExecution";
        m[SingleCheckingParameterError1] = "SingleCheckingParameterError1";
        m[MoveOverLoadingPosition] = "MoveOverLoadingPosition";
        m[MoveOverStandByPosition] = "MoveOverStandByPosition";
        m[MoveOverWastePosition] = "MoveOverWastePosition";
        m[StirrerTestInExecution] = "StirrerTestInExecution";
        m[SingleCheckingParameterError2] = "SingleCheckingParameterError2";
        m[AutoCheckingParameterError] = "AutoCheckingParameterError";
        m[SingleErrorOnConditioningTemper] = "SingleErrorOnConditioningTemper";
        m[AutoErrorOnConditioningTemper] = "AutoErrorOnConditioningTemper";
        m[RunningSeptaTestFunction] = "RunningSeptaTestFunction";
        m[AbortingSeptaTestFunction] = "AbortingSeptaTestFunction";
        m[MovingTurretLeftOrRight] = "MovingTurretLeftOrRight";
        m[MovingTurretStandBy] = "MovingTurretStandBy";
        m[MovingTurretAbort] = "MovingTurretAbort";
        m[SingleMissingVial] = "SingleMissingVial";
        return m;
    }

    static std::map<XHt3000Status1Code, std::string> StatusCodeNameMap()
    {
        std::map<XHt3000Status1Code, std::string> m;
        m[XStatus1StandBy] = "XStatus1StandBy";
        m[XStatus1Running] = "XStatus1Running";
        m[XStatus1WaitGC] = "XStatus1WaitGC";
        m[XStatus1Checking] = "XStatus1Checking";
        m[XStatus1Trigger] = "XStatus1Trigger";
        m[XStatus1PrepFront] = "XStatus1PrepFront";
        m[XStatus1PrepRear] = "XStatus1PrepRear";
        m[XStatus1SyringeWashing] = "XStatus1SyringeWashing";
        m[XStatus1ErrorFind] = "XStatus1ErrorFind";
        m[XStatus1AbortState] = "XStatus1AbortState";
        m[XStatus1SetupRun] = "XStatus1SetupRun";
        m[XStatus1PlateCheck] = "XStatus1PlateCheck";
        m[XStatus1PreWashRun] = "XStatus1PreWashRun";
        m[XStatus1SampleWashRun] = "XStatus1SampleWashRun";
        m[XStatus1BubbleRemove] = "XStatus1BubbleRemove";
        m[XStatus1Ready2Inject] = "XStatus1Ready2Inject";
        m[XStatus1InjectFront] = "XStatus1InjectFront";
        m[XStatus1InjectRear] = "XStatus1InjectRear";
        m[XStatus1InjectDone] = "XStatus1InjectDone";
        m[XStatus1PostWashRun] = "XStatus1PostWashRun";

        return m;
    }
};

int PacketExtention_RequestPacket_Test1();