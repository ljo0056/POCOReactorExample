#pragma once

#include "XHt3000AsPacket.h"

class XHt3000AsInform : public XHt3000AsPacket
{
public:
	BYTE model = XHtHT3000A;	// 30: HT3000A, 31: HT3100A, 32: HT3200A, 37: HT2800T
	// 1.4.2 SAMPLES
	//	Samples differ according to model :

	// @ 3000A
	// - one type of tray(121 positions for 3000A);
	// - no priority vial;
	// - only 2ml vial are allowed;

	// @ 3100A
	// - two types of tray(15 and 20 positions for 3100A);
	// - no priority vial;
	// - only 2ml vial are allowed;

	// @ 3140A
	// - one types of tray(13 positions for 31400A);
	// - no priority vial;
	//   only 2ml vial are allowed;

	// @ 3200A
	// - one type of tray(209 positions for 3200A);
	// - no priority vial;
	// - only 2ml vial are allowed.

	BYTE config = 0;			// 0 bit) 0:right solvent, 1:left solvent
	char version[6] = "02.07";
	DWORD checksum = 2953310223;
	char serial[10] = "H00201723";
	bool set_ethernet = false;
	BYTE ip_address[4] = { 127, 0, 0, 1 };
	BYTE subnet_mask[4] = { 255, 255, 255, 0 };
	BYTE gateway[4] = { 192, 168, 0, 0 };

	XHt3000AsInform();
	
	virtual int Parse(XHt3000PacketCommand code, const ReadOnlySpan& slot) override;
	virtual int Assemble(XHt3000PacketCommand code, std::string& slot) override;
    virtual void PrintLog() override;
};

int Test_XHt3000AsInformExample1();
int Test_XHt3000AsInformExample2();
int Test_XHt3000AsInformExample3();
int Test_XHt3000AsInformExample4();
int Test_XHt3000AsInformExample5();
