#include "pch.h"
#include "XHt3000AsConfig.h"

#include <memory>

XHt3000AsConfig::XHt3000AsConfig()
	: XHt3000AsPacket()
{

}

int XHt3000AsConfig::Parse(XHt3000PacketCommand code, const ReadOnlySpan& slot)
{	
	switch (code)
	{
	case XHt3000CmdSetParameters:
		{
			injection_sync = slot.GetByte(4);            // A ,  4
			injection_mode = slot.GetByte(6);            // B ,  6
			a_start_time = slot.GetByte(8);              // C ,  8
			end_signal = slot.GetByte(10);               // D ,  10
			power_on_restart = slot.GetByte(12);         // E ,  12
			empty_pos = slot.GetByte(14);				 // F ,  14
		}
		break;
		
	case XHt3000CmdSetSetupEx:
	    {
	        sound_enable = slot.GetByte(4); // A ,  4
	        light_enable = slot.GetByte(6); // B ,  6                                        
	        tray_behaviour = slot.GetByte(8);    // C ,  8
	        power_on_restart = slot.GetByte(10); // D ,  10
	        injection_mode = slot.GetByte(12); // E ,  12
	        injection_sync = slot.GetByte(14); // F ,  14
	        a_start_time = slot.GetByte(16); // G ,  16
			empty_pos = slot.GetByte(18); // H ,  18
	        ready_signal_on = slot.GetByte(20); // I ,  20
	        start_signal_on = slot.GetByte(22); // J ,  22
	    }
	    break;

    case XHt3000CmdSetSetupEx2:
        {
            sound_enable = slot.GetByte(4); // A ,  4
            light_enable = slot.GetByte(6); // B ,  6                                        
            tray_behaviour = slot.GetByte(8);    // C ,  8
            power_on_restart = slot.GetByte(10); // D ,  10
            injection_mode = slot.GetByte(12); // E ,  12
            injection_sync = slot.GetByte(14); // F ,  14
            a_start_time = slot.GetByte(16); // G ,  16
            empty_pos = slot.GetByte(18); // H ,  18
            ready_signal_on = slot.GetByte(20); // I ,  20
            start_signal_on = slot.GetByte(22); // J ,  22
            maintenance_warn = slot.GetByte(24); // K ,  24
            ui_type = slot.GetByte(26); // L ,  26
            waiting_position = slot.GetByte(28); // M ,  28
        }
        break;		
	default:
		return 0;
	}

	return slot.GetLength();
}

int XHt3000AsConfig::Assemble(XHt3000PacketCommand code, std::string& slot)
{
	std::string s;	

	switch (code)
	{
	case XHt3000CmdReadBCR:
		{
			// AA BB CC
            s = fmt::format("{:02X}{:02X}{:02X}", bcr_install, bcr_enable, bcr_on_reading_err);
		}
		break;
	case XHt3000CmdReadRFID:
		{
			// AA BBBBBBBBBBBBBBBB CCCCCCCCCCCCCCCC
			s = fmt::format("{:02X}", syringe_rfid_enable);
			s += XHt3000AsPacketExtention::GetHex(syringe_sn, sizeof(syringe_sn) - 1);
			s += XHt3000AsPacketExtention::GetHex(syringe_pn, sizeof(syringe_pn) - 1);
		}
		break;
	case XHt3000CmdSetSetupEx:
		{			
			s += fmt::format("{:02X}", sound_enable); // A ,  2
			s += fmt::format("{:02X}", light_enable); // B ,  2
			s += fmt::format("{:02X}", tray_behaviour); // C ,  2
			s += fmt::format("{:02X}", power_on_restart); // D ,  2
			s += fmt::format("{:02X}", injection_mode); // E ,  2
			s += fmt::format("{:02X}", injection_sync); // F ,  2
			s += fmt::format("{:02X}", a_start_time); // G ,  2
			s += fmt::format("{:02X}", empty_pos); // H ,  2
			s += fmt::format("{:02X}", ready_signal_on); // I ,  2
			s += fmt::format("{:02X}", start_signal_on); // J ,  2			
		}
		break;
	case XHt3000CmdReadSetupEx:
		{
			s += fmt::format("{:04X}", tray); // A ,  4
			s += fmt::format("{:04X}", vial); // B ,  4
			s += fmt::format("{:04X}", depth); // C ,  4
			s += fmt::format("{:02X}", syringe); // D ,  2
			s += fmt::format("{:06X}", syringe_volume); // E ,  6
			s += XHt3000AsPacketExtention::GetHex(analyser_family, sizeof(analyser_family) - 1); // F ,  80
			s += XHt3000AsPacketExtention::GetHex(analyser_model, sizeof(analyser_model) - 1);  // G ,  80
			s += fmt::format("{:02X}", injectors); // H ,  2
			s += fmt::format("{:02X}", inj0_speed); // I ,  2
			s += fmt::format("{:04X}", inj0_depth); // J ,  4
			s += fmt::format("{:02X}", inj1_speed); // K ,  2
			s += fmt::format("{:04X}", inj1_depth); // L ,  4
			s += fmt::format("{:02X}", sound_enable); // M ,  2
			s += fmt::format("{:02X}", light_enable); // N ,  2
			s += fmt::format("{:02X}", tray_behaviour); // O ,  2
			s += fmt::format("{:02X}", power_on_restart); // P ,  2
			s += fmt::format("{:02X}", injection_mode); // Q ,  2
			s += fmt::format("{:02X}", injection_sync); // R ,  2
			s += fmt::format("{:02X}", a_start_time); // S ,  2
			s += fmt::format("{:02X}", empty_pos); // T ,  2
			s += fmt::format("{:02X}", run_all_sample); // U ,  2
			s += fmt::format("{:02X}", ready_signal_on); // V ,  2
			s += fmt::format("{:02X}", start_signal_on); // W ,  2
			s += fmt::format("{:02X}", plunger_speed); // X ,  2
			s += fmt::format("{:04X}", solvent_depth); // Y ,  4
			s += fmt::format("{:06X}", solvent_speed); // Z ,  6
			s += fmt::format("{:02X}", touch_plunger_zero); // A ,  2
			s += fmt::format("{:08X}", plunger_counter); // B ,  8
			s += fmt::format("{:08X}", plunger_counter_limit); // C ,  8
			s += fmt::format("{:08X}", injector_counter); // D ,  8
			s += fmt::format("{:08X}", injector_counter_limit); // E ,  8
		}
		break;
	case XHt3000CmdSetSetupEx2:
		{			 
			s += fmt::format("{:02X}", sound_enable); // A ,  2
			s += fmt::format("{:02X}", light_enable); // B ,  2
			s += fmt::format("{:02X}", tray_behaviour); // C ,  2
			s += fmt::format("{:02X}", power_on_restart); // D ,  2
			s += fmt::format("{:02X}", injection_mode); // E ,  2
			s += fmt::format("{:02X}", injection_sync); // F ,  2
			s += fmt::format("{:02X}", a_start_time); // G ,  2
			s += fmt::format("{:02X}", empty_pos); // H ,  2
			s += fmt::format("{:02X}", ready_signal_on); // I ,  2
			s += fmt::format("{:02X}", start_signal_on); // J ,  2
			s += fmt::format("{:02X}", maintenance_warn); // K ,  2
			s += fmt::format("{:02X}", ui_type); // L ,  2
			s += fmt::format("{:02X}", waiting_position); // M ,  2
		}
		break;
	case XHt3000CmdReadSetupEx2:
		{
			s += fmt::format("{:04X}", tray); // A ,  4
			s += fmt::format("{:04X}", vial); // B ,  4
			s += fmt::format("{:04X}", depth); // C ,  4
			s += fmt::format("{:02X}", syringe); // D ,  2
			s += fmt::format("{:06X}", syringe_volume); // E ,  6
			s += XHt3000AsPacketExtention::GetHex(analyser_family, sizeof(analyser_family) - 1); // F ,  80
			s += XHt3000AsPacketExtention::GetHex(analyser_model, sizeof(analyser_model) - 1);  // G ,  80
			s += fmt::format("{:02X}", injectors); // H ,  2
			s += fmt::format("{:02X}", inj0_speed); // I ,  2
			s += fmt::format("{:04X}", inj0_depth); // J ,  4
			s += fmt::format("{:02X}", inj1_speed); // K ,  2
			s += fmt::format("{:04X}", inj1_depth); // L ,  4
			s += fmt::format("{:02X}", sound_enable); // M ,  2
			s += fmt::format("{:02X}", light_enable); // N ,  2
			s += fmt::format("{:02X}", tray_behaviour); // O ,  2
			s += fmt::format("{:02X}", power_on_restart); // P ,  2
			s += fmt::format("{:02X}", injection_mode); // Q ,  2
			s += fmt::format("{:02X}", injection_sync); // R ,  2
			s += fmt::format("{:02X}", a_start_time); // S ,  2
			s += fmt::format("{:02X}", empty_pos); // T ,  2
			s += fmt::format("{:02X}", run_all_sample); // U ,  2
			s += fmt::format("{:02X}", ready_signal_on); // V ,  2
			s += fmt::format("{:02X}", start_signal_on); // W ,  2
			s += fmt::format("{:02X}", plunger_speed); // X ,  2
			s += fmt::format("{:04X}", solvent_depth); // Y ,  4
			s += fmt::format("{:06X}", solvent_speed); // Z ,  6
			s += fmt::format("{:06X}", waste_speed); // A ,  6
			s += fmt::format("{:02X}", touch_plunger_zero); // B ,  2
			s += fmt::format("{:02X}", ui_type); // C ,  2
			s += fmt::format("{:02X}", maintenance_warn); // D ,  2
			s += fmt::format("{:08X}", plunger_counter); // E ,  8
			s += fmt::format("{:08X}", plunger_counter_limit); // F ,  8
			s += fmt::format("{:08X}", injector_counter); // G ,  8
			s += fmt::format("{:08X}", injector_counter_limit); // H ,  8
			s += fmt::format("{:02X}", waiting_position); // I ,  2
			s += fmt::format("{:02X}", large_volume); // J ,  2
		}
		break;
	default:
		return 0;
	}

	slot = fmt::format("{:04X}{}", s.length() + 16, s);

	return (int)slot.length();
}

void XHt3000AsConfig::PrintLog()
{
	LOGI << fmt::format(
		"XHt3000AsConfig  tray[{}] vial[{}] depth[{}] syringe[{}] syringe_volume[{}] "
		"analyser_family[{}] analyser_model[{}] injectors[{}] inj0_speed[{}] inj0_depth[{}] "
		"inj1_speed[{}] inj1_depth[{}] plunger_speed[{}] solvent_depth[{}] "
		"solvent_speed[{}] waste_speed[{}] touch_plunger_zero[{}] ui_type[{}] "
		"bcr_install[{}] syringe_sn[{}] syringe_pn[{}] sound_enable[{}] "
		"light_enable[{}] tray_behaviour[{}] power_on_restart[{}] injection_sync[{}] "
		"injection_mode[{}] a_start_time[{}] empty_pos[{}] run_all_sample[{}] "
		"ready_signal_on[{}] start_signal_on[{}] maintenance_warn[{}] plunger_counter[{}] "
		"plunger_counter_limit[{}] injector_counter[{}] injector_counter_limit[{}] bcr_enable[{}] "
		"bcr_on_reading_err[{}] syringe_rfid_enable[{}] waiting_position[{}] large_volume[{}] ",
        tray, vial, depth, syringe, syringe_volume,
        analyser_family, analyser_model, injectors, inj0_speed, inj0_depth,
        inj1_speed, inj1_depth, plunger_speed, solvent_depth,
        solvent_speed, waste_speed, touch_plunger_zero, ui_type,
        bcr_install, syringe_sn, syringe_pn, sound_enable,
        light_enable, tray_behaviour, power_on_restart, injection_sync,
        injection_mode, a_start_time, empty_pos, run_all_sample,
        ready_signal_on, start_signal_on, maintenance_warn, plunger_counter,
        plunger_counter_limit, injector_counter, injector_counter_limit, bcr_enable,
        bcr_on_reading_err, syringe_rfid_enable, waiting_position, large_volume
	);
}
