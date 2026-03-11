#include "pch.h"
#include "XHt3000AsSetup.h"

#include <memory>

XHt3000AsSetup::XHt3000AsSetup()
    : XHt3000AsPacket()
{
    // XHt3000CmdWriteMethod
    {
        byte buffer[] = {
            0x24, 0x31, 0x33, 0x30, 0x30, 0x30, 0x30, 0x30,
            0x30, 0x30, 0x31, 0x30, 0x30, 0x34, 0x41, 0x30,
            0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30,
            0x30, 0x43, 0x38, 0x30, 0x30, 0x43, 0x38, 0x30,
            0x30, 0x35, 0x30, 0x30, 0x30, 0x36, 0x34, 0x32,
            0x37, 0x31, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30,
            0x30, 0x30, 0x30, 0x30, 0x30, 0x39, 0x36, 0x30,
            0x30, 0x30, 0x30, 0x31, 0x30, 0x30, 0x30, 0x34,
            0x36, 0x30, 0x30, 0x33, 0x32, 0x30, 0x30, 0x30,
            0x30, 0x37, 0x38, 0x0d
        };

        ReadOnlySpan slot(buffer, sizeof(buffer));
        XHt3000AsPacketExtention::PerformParse(this, slot);
    }    

    // XHt3000CmdSingleRunEx2
    {
        byte buffer[] = {
            0x24, 0x33, 0x34, 0x30, 0x30, 0x30, 0x30, 0x30,
            0x30, 0x30, 0x31, 0x30, 0x30, 0x31, 0x45, 0x30,
            0x30, 0x30, 0x30, 0x46, 0x46, 0x30, 0x30, 0x30,
            0x30, 0x30, 0x30, 0x30, 0x31, 0x37, 0x32, 0x0d
        };

        ReadOnlySpan slot(buffer, sizeof(buffer));
        XHt3000AsPacketExtention::PerformParse(this, slot);
    }
}

float XHt3000AsSetup::GetVolume(UINT value)
{
	value = min(syringe_volume, value);

	float fv = (float)value / 100.0f;
	switch (syringe_volume) {
	case 50:
	case 100:
	case 500:
		break;
	case 1000:
	case 2500:
	case 5000:
		fv = (float)(value / 10) / 10.0f;
		break;
	case 10000:
	case 25000:
	case 50000:
		fv = (float)(value / 100);
		break;
	}
	return fv;
}
float XHt3000AsSetup::GetSpeed(UINT value)
{
	value = min(syringe_volume * 5, value);

	float fv = (float)value / 100.0f;
	switch (syringe_volume) {
	case 50:
	case 100:
	case 500:
		break;
	case 1000:
	case 2500:
	case 5000:
		fv = (float)(value / 10) / 10.0f;
		break;
	case 10000:
	case 25000:
	case 50000:
		fv = (float)(value / 100);
		break;
	}
	return fv;
}

int XHt3000AsSetup::Parse(XHt3000PacketCommand code, const ReadOnlySpan& slot)
{    
	switch (code)
	{        
    case XHt3000CmdWriteMethod:
        {
            method = slot.GetByte(4); // A ,  4
            add_istd = slot.GetByte(6); // B ,  6
            pre_wash_count = slot.GetByte(8, 1); // C ,  8
            post_wash_count = slot.GetByte(9, 1); // D ,  9
            pre_wash_mode = slot.GetByte(10, 1); // E ,  10
            post_wash_mode = slot.GetByte(11, 1); // F ,  11
            pre_wash_volume = slot.GetUShort(12); // G ,  12
            post_wash_volume = slot.GetUShort(16); // H ,  16
            sample_wash_count = slot.GetByte(20); // I ,  20
            pullup_count = slot.GetByte(22, 1); // J ,  22
            viscosity_delay = slot.GetByte(23, 1); // K ,  23
            sample_fill_speed = slot.GetUShort(24); // L ,  24
            sample_inject_speed = slot.GetUShort(28); // M ,  28
            sample_volume = slot.GetUShort(32); // N ,  32
            air_volume = slot.GetUShort(36); // O ,  36
            sample_fill_volume = slot.GetUShort(40); // P ,  40
            pre_dwell_time = slot.GetByte(44); // Q ,  44
            post_dwell_time = slot.GetByte(46); // R ,  46
            air_gap_mode = slot.GetByte(48); // S ,  48
            istd_volume = slot.GetUShort(50); // T ,  50
            istd_air_volume = slot.GetUShort(54); // U ,  54
            // dummy // V ,  58
        }
        break;
    case XHt3000CmdWriteMethodEx:
        {
            method = slot.GetByte(4); // A ,  4
            slot.GetString(6, 32, method_name, sizeof(method_name)); // B ,  6
            syringe_volume = slot.GetUInt32(38, 6); // C ,  38
            analysis_time = slot.GetUShort(44); // D ,  44
            pre_wash_count = slot.GetByte(48, 1); // E ,  48
            post_wash_count = slot.GetByte(49, 1); // F ,  49
            pre_wash_mode = slot.GetByte(50, 1); // G ,  50
            post_wash_mode = slot.GetByte(51, 1); // H ,  51
            pre_wash_volume = slot.GetUShort(52); // I ,  52
            post_wash_volume = slot.GetUShort(56); // J ,  56
            sample_wash_count = slot.GetByte(60); // K ,  60
            pullup_count = slot.GetByte(62, 1); // L ,  62
            viscosity_delay = slot.GetByte(63, 1); // M ,  63
            sample_fill_speed = slot.GetUShort(64); // N ,  64
            sample_inject_speed = slot.GetUShort(68); // O ,  68
            sample_volume = slot.GetUShort(72); // P ,  72
            air_volume = slot.GetUShort(76); // Q ,  76
            sample_fill_volume = slot.GetUShort(80); // R ,  80
            pre_dwell_time = slot.GetByte(84); // S ,  84
            post_dwell_time = slot.GetByte(86); // T ,  86
            air_gap_mode = slot.GetByte(88); // U ,  88
            add_istd = slot.GetByte(90); // V ,  90
            istd_volume = slot.GetUShort(92); // W ,  92
            istd_air_volume = slot.GetUShort(96); // X ,  96
        }
        break;    
    
    case XHt3000CmdSingleRunEx2:
        {
            method = slot.GetByte(4); // A ,  4
            sample_number = slot.GetByte(6); // B ,  6
            istd = slot.GetByte(8); // C ,  8
            pre_solvent = slot.GetByte(10); // D ,  10
            post_solvent = slot.GetByte(12); // E ,  12
            injection_sync = slot.GetByte(14); // F ,  14
            tray_behaviour = slot.GetByte(16); // G ,  16

            LOGI << fmt::format("XHt3000CmdSingleRunEx2 method[{}] sampler_number[{}] istd[{}] pre_solvent[{}] post_solvent[{}] sync[{}] tray_behaviour[{}]",
                method, sample_number, istd, pre_solvent, post_solvent, injection_sync, tray_behaviour);
        }
        break;
	default:
		return false;
	}

	return slot.GetLength();
}

int XHt3000AsSetup::Assemble(XHt3000PacketCommand code, std::string& slot)
{
	std::string s;	

	switch (code)
	{    
    case XHt3000CmdReadMethod:
    case XHt3000CmdWriteMethod:
        {
            s += fmt::format("{:02X}", method); // A ,  2
            s += fmt::format("{:02X}", add_istd); // B ,  2
            s += fmt::format("{:01X}", pre_wash_count); // C ,  1
            s += fmt::format("{:01X}", post_wash_count); // D ,  1
            s += fmt::format("{:01X}", pre_wash_mode); // E ,  1
            s += fmt::format("{:01X}", post_wash_mode); // F ,  1
            s += fmt::format("{:04X}", pre_wash_volume); // G ,  4
            s += fmt::format("{:04X}", post_wash_volume); // H ,  4
            s += fmt::format("{:02X}", sample_wash_count); // I ,  2
            s += fmt::format("{:01X}", pullup_count); // J ,  1
            s += fmt::format("{:01X}", viscosity_delay); // K ,  1
            s += fmt::format("{:04X}", sample_fill_speed); // L ,  4
            s += fmt::format("{:04X}", sample_inject_speed); // M ,  4
            s += fmt::format("{:04X}", sample_volume); // N ,  4
            s += fmt::format("{:04X}", air_volume); // O ,  4
            s += fmt::format("{:04X}", sample_fill_volume); // P ,  4
            s += fmt::format("{:02X}", pre_dwell_time); // Q ,  2
            s += fmt::format("{:02X}", post_dwell_time); // R ,  2
            s += fmt::format("{:02X}", air_gap_mode); // S ,  2
            s += fmt::format("{:04X}", istd_volume); // T ,  4
            s += fmt::format("{:04X}", istd_air_volume); // U ,  4
            s += fmt::format("{:04X}", 0); // V ,  4 // dummy
        }
        break;
    case XHt3000CmdReadMethodEx:
    case XHt3000CmdWriteMethodEx:
        {
            s += fmt::format("{:02X}", method); // A ,  2            
            s += XHt3000AsPacketExtention::GetHex(method_name, sizeof(method_name) - 1);            
            s += fmt::format("{:06X}", syringe_volume); // C ,  6
            s += fmt::format("{:04X}", analysis_time); // D ,  4
            s += fmt::format("{:01X}", pre_wash_count); // E ,  1
            s += fmt::format("{:01X}", post_wash_count); // F ,  1
            s += fmt::format("{:01X}", pre_wash_mode); // G ,  1
            s += fmt::format("{:01X}", post_wash_mode); // H ,  1
            s += fmt::format("{:04X}", pre_wash_volume); // I ,  4
            s += fmt::format("{:04X}", post_wash_volume); // J ,  4
            s += fmt::format("{:02X}", sample_wash_count); // K ,  2
            s += fmt::format("{:01X}", pullup_count); // L ,  1
            s += fmt::format("{:01X}", viscosity_delay); // M ,  1
            s += fmt::format("{:04X}", sample_fill_speed); // N ,  4
            s += fmt::format("{:04X}", sample_inject_speed); // O ,  4
            s += fmt::format("{:04X}", sample_volume); // P ,  4
            s += fmt::format("{:04X}", air_volume); // Q ,  4
            s += fmt::format("{:04X}", sample_fill_volume); // R ,  4
            s += fmt::format("{:02X}", pre_dwell_time); // S ,  2
            s += fmt::format("{:02X}", post_dwell_time); // T ,  2
            s += fmt::format("{:02X}", air_gap_mode); // U ,  2
            s += fmt::format("{:02X}", add_istd); // V ,  2
            s += fmt::format("{:04X}", istd_volume); // W ,  4
            s += fmt::format("{:04X}", istd_air_volume); // X ,  4
        }
        break;
    case XHt3000CmdSingleRunEx2:
        {
            s += fmt::format("{:02X}", method); // A ,  2
            s += fmt::format("{:02X}", sample_number); // B ,  2
            s += fmt::format("{:02X}", istd); // C ,  2
            s += fmt::format("{:02X}", pre_solvent); // D ,  2
            s += fmt::format("{:02X}", post_solvent); // E ,  2
            s += fmt::format("{:02X}", injection_sync); // F ,  2
            s += fmt::format("{:02X}", tray_behaviour); // G ,  2
        }
        break;   
	default:
		return 0;
	}

    slot = fmt::format("{:04X}{}", s.length() + 16, s);

	return (int)slot.length();
}

void XHt3000AsSetup::PrintLog()
{
    LOGI << fmt::format(
        "XHt3000AsSetup  sync[{}] mode[{}] delay[{}] end_signal[{}] empty_position_syncro[{}] run_all_samples[{}] ready_signal_on[{}] start_signal_on[{}] "
        "save_in_eeprom[{}] method[{}] method_name[{}] syringe_volume[{}] analysis_time[{}] add_istd[{}] "
        "pre_wash_mode[{}] pre_wash_count[{}] pre_wash_volume[{}] "
        "post_wash_mode[{}] post_wash_count[{}] post_wash_volume[{}] "
        "sample_wash_count[{}] sample_fill_volume[{}] pullup_count[{}] viscosity_delay[{}] "
        "sample_fill_speed[{}] sample_inject_speed[{}] sample_volume[{}] air_volume[{}] "
        "pre_dwell_time[{}] post_dwell_time[{}] "
        "air_gap_mode[{}] istd_volume[{}] istd_air_volume[{}] "
        "sequence_name[{}] pre_solvent[{}] post_solvent[{}] istd[{}] "
        "tray_behaviour[{}] power_on_restart[{}] sample_number[{}] ",        
        injection_sync, injection_mode, a_start_time, end_signal, empty_position_syncro, run_all_samples, ready_signal_on, start_signal_on,
        save_in_eeprom, method, method_name, syringe_volume, analysis_time, add_istd,
        pre_wash_mode, pre_wash_count, pre_wash_volume,
        post_wash_mode, post_wash_count, post_wash_volume,
        sample_wash_count, sample_fill_volume, pullup_count, viscosity_delay,
        sample_fill_speed, sample_inject_speed, sample_volume, air_volume,
        pre_dwell_time, post_dwell_time,
        air_gap_mode, istd_volume, istd_air_volume,
        sequence_name, pre_solvent, post_solvent, istd,
        tray_behaviour, power_on_restart, sample_number
        );
}