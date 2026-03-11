#pragma once

#include <algorithm>
#include "XHt3000AsPacket.h"

class XHt3000AsSetup : public XHt3000AsPacket
{
public:
	// READ and WRITE Parameters
	BYTE injection_sync = XInjectSyncNormal;				// XHt3000InjectSync	
	BYTE injection_mode = XInjectModeFront;				// XHt3000InjectMode injector가 하나면 무조건 Normal	
	BYTE a_start_time = 10;				// if sync = XInjectSyncAStart	(0.1 sec) A-Start time
	BYTE end_signal = 0;		// 0: No, 1: Yes
	BYTE empty_position_syncro = 0;
	BYTE run_all_samples = 0;	// 0: no, 1: yes
	BYTE ready_signal_on = 1;	// 0: high, 1: low
	BYTE start_signal_on = 1;	// 0: open, 1: close

	// Method
	BYTE save_in_eeprom = 0;
	BYTE method = 0;			// 0~9
	char method_name[17] = { 0, };
	UINT syringe_volume = 0;	// 0.01 ul
	WORD analysis_time = 0;		// 0-59940 s

	BYTE add_istd = 0;			// 0: No, 1: Yes

	BYTE pre_wash_mode = 0;		// 0: injection, 1: sample, 2: step
	BYTE pre_wash_count = 0;	// 0~15
	WORD pre_wash_volume = 200;	// 0.01 ul

	BYTE post_wash_mode = 0;
	BYTE post_wash_count = 0;
	WORD post_wash_volume = 200;

	BYTE sample_wash_count = 0;	// 0~15
	WORD sample_fill_volume = 150; // 0.01 ul ISTD가 아닐때만 유효함
	BYTE pullup_count = 5;		// 0~15
	BYTE viscosity_delay = 0;	// 0~15 sec

	WORD sample_fill_speed = 100;	// 0.01 ul/s
	WORD sample_inject_speed = 10000;
	WORD sample_volume = 0;		// 0.01 ul 주입량
	WORD air_volume = 0;		// 0.01 ul

	BYTE pre_dwell_time = 0;	// (0 ~ 99) seconds
	BYTE post_dwell_time = 0;

	BYTE air_gap_mode = 16;		// 0:post, (1<<4):double		// ISTD mode 일때만
	WORD istd_volume = 70;		// (0~50000)
	WORD istd_air_volume = 50;	// (0~50000)

	// Sequence
	char sequence_name[17] = { 0, };
	BYTE pre_solvent = 0;		// 0-5(A-F), 6-35 
	BYTE post_solvent = 0;
	BYTE istd = 0xff;			// 0xff : not use

	BYTE tray_behaviour = 1;    // 0: close, 1: open
	BYTE power_on_restart = 0;  // 0: disable, 1: enable => set always disable

	BYTE sample_number = 0;		// zero base    

public:	
	XHt3000AsSetup();

	float GetVolume(UINT value);	
	float GetSpeed(UINT value);	

	virtual int Parse(XHt3000PacketCommand code, const ReadOnlySpan& slot) override;
	virtual int Assemble(XHt3000PacketCommand code, std::string& slot) override;
	virtual void PrintLog() override;
};
