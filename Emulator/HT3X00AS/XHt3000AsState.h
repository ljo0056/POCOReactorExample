#pragma once

#include <algorithm>
#include "XHt3000AsPacket.h"

class XHt3000AsState : public XHt3000AsPacket
{
public:
	UINT total = 0;
	WORD errors = 0;					// XHt3000Error2Code

	BYTE last_seq_step = 0;
	BYTE cur_seq_step = 0;		// (0~16), FF: AUTORUN NOT ACTIVE, >127: in checking phase
	// if last_step < step, autorun is completed but for waiting READY	
	BYTE vial = 0;
	BYTE last_vial = 0;	// if last_vial < vial, steop is completed and sampler is skipping "next sequence" step
	BYTE repeats = 0;	// remaining repetitions (0~9) (0=end of repetitions)
	BYTE istd = 0;		// (220-225, A-F) FF: associated method doesn't use istd
	BYTE method = 0;
	BYTE pre_solvent = 0;	// (0-5, A-F)
	BYTE post_solvent = 0;
	BYTE wash_repeats = 0;	// (1-15) valid only if sampler is washing
	BYTE washing_solvent = 0;	// (0-5, A-F)
	WORD wash_volume = 100;	// 0.01 ul
	WORD remain_analysis_time = 0;	// 0-59940 s

	BYTE bcr_type;		// 0: standard, 1: test
	char barcode[65] = { 0, };

public:	
	XHt3000AsState();

	void Reset();

	virtual int Parse(XHt3000PacketCommand code, const ReadOnlySpan& slot) override;
	virtual int Assemble(XHt3000PacketCommand code, std::string& slot) override;
	virtual void PrintLog() override;
};

int Test_XHt3000AsState1();