#include "pch.h"
#include "XHt3000AsState.h"

#include <memory>

namespace plog
{
    Record& operator<<(Record& record, const XHt3000AsState& data)
    {
        std::string s = fmt::format(
            "XHt3000AsState  error1[{}] error2[{}] status1[{}] status1_new[{}] status2[{}]"
            "total[{}] errors[{}]"
            "last_seq_step[{}] cur_seq_step[{}] vial[{}] last_vial[{}] repeats[{}]"
            "istd[{}] method[{}]"
            "pre_solvent[{}] post_solvent[{}]"
            "wash_repeats[{}] washing_solvent[{}] wash_volume[{}]"
            "bcr_type[{}] barcode[{}]",
            data.error1, data.error2, data.status1, data.status1_new, data.status2,
            data.total, data.errors,
            data.last_seq_step, data.cur_seq_step, data.vial, data.last_vial, data.repeats,
            data.istd, data.method,
            data.pre_solvent, data.post_solvent,
            data.wash_repeats, data.washing_solvent, data.wash_volume,
            data.bcr_type, data.barcode);

        return record << s;
    }
}

XHt3000AsState::XHt3000AsState()
    : XHt3000AsPacket()
{
    
}

void XHt3000AsState::Reset()
{
    status1 = 0;
    status1_new = 0;
    error1 = 0;
    error2 = 0;

    total = 0;
    errors = 0;

    last_seq_step = 0;
    cur_seq_step = 0;
    
    vial = 0;
    last_vial = 0;
    repeats = 0;
    istd = 0;
    method = 0;
    pre_solvent = 0;
    post_solvent = 0;
    wash_repeats = 0;
    washing_solvent = 0;
    wash_volume = 100;
    remain_analysis_time = 0;

    bcr_type;
    memset(barcode, 0, sizeof(barcode));    
}

int XHt3000AsState::Parse(XHt3000PacketCommand code, const ReadOnlySpan& slot)
{    
    BYTE* error = GetError();
    BYTE* status = GetStatus();	

	switch (code)
	{
    case XHt3000CmdReadSampler:
    case XHt3000CmdReadSamplerEx:
        {            
            total = slot.GetUShort(4);           // A ,  4
            errors = slot.GetUShort(8);          // B ,  8
            last_seq_step = slot.GetByte(12);    // C ,  12
            cur_seq_step = slot.GetByte(14);     // D ,  14
            vial = slot.GetByte(16);             // E ,  16
            last_vial = slot.GetByte(18);        // F ,  18
            repeats = slot.GetByte(20);          // G ,  20
            istd = slot.GetByte(22);             // H ,  22
            method = slot.GetByte(24);           // I ,  24
            pre_solvent = slot.GetByte(26);      // J ,  26
            post_solvent = slot.GetByte(28);     // K ,  28
            wash_repeats = slot.GetByte(30);     // L ,  30
            washing_solvent = slot.GetByte(32);  // M ,  32
            wash_volume = slot.GetUShort(34);    // N ,  34
        }
        break;
    case XHt3000CmdReadSamplerEx2:
        {
            total = slot.GetUInt32(4, 6);        // A ,  4
            errors = slot.GetUShort(10);         // B ,  10
            cur_seq_step = slot.GetByte(14);     // C ,  14
            last_seq_step = slot.GetByte(16);    // D ,  16
            vial = slot.GetByte(18);             // E ,  18
            last_vial = slot.GetByte(20);        // F ,  20
            repeats = slot.GetByte(22);          // G ,  22
            istd = slot.GetByte(24);             // H ,  24
            method = slot.GetByte(26);           // I ,  26
            pre_solvent = slot.GetByte(28);      // J ,  28
            post_solvent = slot.GetByte(30);     // K ,  30
            wash_repeats = slot.GetByte(32);     // L ,  32
            washing_solvent = slot.GetByte(34);  // M ,  34
            wash_volume = slot.GetUShort(36);    // N ,  36
            remain_analysis_time = slot.GetUShort(40);   // O ,  40
        }
        break;
    case XHt3000CmdReadLastBCR:
        {
            bcr_type = slot.GetByte(4);
            slot.GetString(6, 128, barcode, sizeof(barcode));
        }
        break;
	default:
		return false;
	}

	return slot.GetLength();
}

int XHt3000AsState::Assemble(XHt3000PacketCommand code, std::string& slot)
{
	std::string s;

    error[0] = error1;
    error[1] = error2;
    status[0] = status1;
    status[1] = status2;

	switch (code)
	{
    case XHt3000CmdReadSampler:
        {
            s += fmt::format("{:04X}", total); // A ,  4
            s += fmt::format("{:04X}", errors); // B ,  4
            s += fmt::format("{:02X}", last_seq_step); // C ,  2
            s += fmt::format("{:02X}", cur_seq_step); // D ,  2
            s += fmt::format("{:02X}", vial); // E ,  2
            s += fmt::format("{:02X}", last_vial); // F ,  2
            s += fmt::format("{:02X}", repeats); // G ,  2
            s += fmt::format("{:02X}", istd); // H ,  2
            s += fmt::format("{:02X}", method); // I ,  2
            s += fmt::format("{:02X}", pre_solvent); // J ,  2
            s += fmt::format("{:02X}", post_solvent); // K ,  2
            s += fmt::format("{:02X}", wash_repeats); // L ,  2
            s += fmt::format("{:02X}", washing_solvent); // M ,  2
            s += fmt::format("{:04X}", wash_volume); // N ,  4
        }
        break;
    case XHt3000CmdReadSamplerEx:
        {
            s += fmt::format("{:04X}", total); // A ,  4
            s += fmt::format("{:04X}", errors); // B ,  4
            s += fmt::format("{:02X}", last_seq_step); // C ,  2
            s += fmt::format("{:02X}", cur_seq_step); // D ,  2
            s += fmt::format("{:02X}", vial); // E ,  2
            s += fmt::format("{:02X}", last_vial); // F ,  2
            s += fmt::format("{:02X}", repeats); // G ,  2
            s += fmt::format("{:02X}", istd); // H ,  2
            s += fmt::format("{:02X}", method); // I ,  2
            s += fmt::format("{:02X}", pre_solvent); // J ,  2
            s += fmt::format("{:02X}", post_solvent); // K ,  2
            s += fmt::format("{:02X}", wash_repeats); // L ,  2
            s += fmt::format("{:02X}", washing_solvent); // M ,  2
            s += fmt::format("{:04X}", wash_volume); // N ,  4
        }
        break;
    case XHt3000CmdReadSamplerEx2:
        {
            s += fmt::format("{:06X}", total); // A ,  6
            s += fmt::format("{:04X}", errors); // B ,  4
            s += fmt::format("{:02X}", cur_seq_step); // C ,  2
            s += fmt::format("{:02X}", last_seq_step); // D ,  2
            s += fmt::format("{:02X}", vial); // E ,  2
            s += fmt::format("{:02X}", last_vial); // F ,  2
            s += fmt::format("{:02X}", repeats); // G ,  2
            s += fmt::format("{:02X}", istd); // H ,  2
            s += fmt::format("{:02X}", method); // I ,  2
            s += fmt::format("{:02X}", pre_solvent); // J ,  2
            s += fmt::format("{:02X}", post_solvent); // K ,  2
            s += fmt::format("{:02X}", wash_repeats); // L ,  2
            s += fmt::format("{:02X}", washing_solvent); // M ,  2
            s += fmt::format("{:04X}", wash_volume); // N ,  4
            s += fmt::format("{:04X}", remain_analysis_time); // O ,  4
        }
        break;
    case XHt3000CmdAbort:        
        break;
	default:
		return 0;
	}

    slot = fmt::format("{:04X}{}", s.length() + 16, s);

	return (int)slot.length();
}

void XHt3000AsState::PrintLog()
{
    LOGI << fmt::format(
        "XHt3000AsState  error1[{}] error2[{}] status1[{}] status1_new[{}] status2[{}] "
        "total[{}] errors[{}] "
        "last_seq_step[{}] cur_seq_step[{}] vial[{}] last_vial[{}] repeats[{}] "
        "istd[{}] method[{}] "
        "pre_solvent[{}] post_solvent[{}] "
        "wash_repeats[{}] washing_solvent[{}] wash_volume[{}] "
        "bcr_type[{}] barcode[{}]",
        error1, error2, status1, status1_new, status2,
        total, errors,
        last_seq_step, cur_seq_step, vial, last_vial, repeats,
        istd, method,
        pre_solvent, post_solvent,
        wash_repeats, washing_solvent, wash_volume,
        bcr_type, barcode);
}

int Test_XHt3000AsState1()
{
    XHt3000AsState state;
    std::string response;
    XHt3000AsPacketExtention::PerformAssemble(&state, XHt3000CmdAbort, response);
    return 0;
}