#pragma once

#include <algorithm>
#include "XHt3000AsPacket.h"

class XHt3000AsConfig : public XHt3000AsPacket
{
public:
	// READ ONLY
	WORD tray = 121;
	WORD vial = 20; 				// 0.1 ml
	WORD depth = 300;				// (0~30.0 mm) 0.1 mm 
	BYTE syringe = 2;				// syringe type
									// 0: 1 ul, 1: 5 ul, 2: 10 ul, 3: 25 ul, 4: 50 ul, 5: 100 ul, 6: 250 ul, 7: 500 ul, 8: 0.5 ul
	UINT syringe_volume = 1000;		// 0.01 ul	
	char analyser_family[41] = { 0, };
	char analyser_model[41] = { 0, };

	BYTE injectors = 1;				// 0: 1 injector, 1: 2 injectors // Number of Injectors
	BYTE inj0_speed = 3;			// 0: very low, 1: low, 2: medium, 3: high, 4: very high
	WORD inj0_depth = 450;			// (0-99.0) mm	0.1 mm
	// these are valid when injectors is 1 !!!
	BYTE inj1_speed = 1;			// 0: very low, 1: low, 2: medium, 3: high, 4: very high
	WORD inj1_depth = 20;			// (0-99.0) mm 0.1 mm
	BYTE plunger_speed = 0;			// 0: standard, 1: high
	WORD solvent_depth = 430;		// (0-99.0) mm 0.1 mm
	UINT solvent_speed = 350;		// 0.01 - 2500 ul/s
	UINT waste_speed = 0;			// 0.01 - 2500 ul/s
	BYTE touch_plunger_zero = 1;	// 0: manual, 1: automatic
	BYTE ui_type = 0;				// 0: standard, 1: quick start 2: remote  1.11 부터 R/W  
	BYTE bcr_install = 0;			// 0: no, 1: yes
	char syringe_sn[9] = { 0, };
	char syringe_pn[9] = { 0, };

	// READ & WRITE
	BYTE sound_enable = 0;			// 0: level0, 1: level1, level2
	BYTE light_enable = 0;			// 0: disable, 1: enable
	BYTE tray_behaviour = 1;		// 0: close, 1: open
	BYTE power_on_restart = 0;		// 0: disable, 1: enable => set always disable

	BYTE injection_sync = XInjectSyncNormal;           // XHt3000InjectSync	
	BYTE injection_mode = XInjectModeFront;	           // XHt3000InjectMode injector가 하나면 무조건 Normal	
	BYTE a_start_time = 10;				               // if sync = XInjectSyncAStart	(0.1 sec) A-Start time	
	BYTE end_signal = 0;		// 0: No, 1: Yes

	BYTE empty_pos = 0;
	BYTE run_all_sample = 1;
	BYTE ready_signal_on = 1;
	BYTE start_signal_on = 1;
	BYTE maintenance_warn = 0;		// 0: normal, 1: only at start-up
	UINT plunger_counter = 845;		// 0: off, 1~
	UINT plunger_counter_limit = 25000;
	UINT injector_counter = 3125;		// 0: off, 1~
	UINT injector_counter_limit = 0;
	BYTE bcr_enable = 0;			// 0: no, 1: yes
	BYTE bcr_on_reading_err = 0;	// 0: skip, 1: process
	BYTE syringe_rfid_enable = 0;	// 0: disable, 1: enable
	BYTE waiting_position = 0;		// 0: in vial, 1: on injector

	BYTE large_volume = 0;			// 0: Normal, 1:Large volume => read only

public:	
	XHt3000AsConfig();

	virtual int Parse(XHt3000PacketCommand code, const ReadOnlySpan& slot) override;
	virtual int Assemble(XHt3000PacketCommand code, std::string& slot) override;
    virtual void PrintLog() override;
};
