#include "pch.h"
#include "XHt3000AsInform.h"

#include <memory>

XHt3000AsInform::XHt3000AsInform()
	: XHt3000AsPacket()
{
 
}

int XHt3000AsInform::Parse(XHt3000PacketCommand code, const ReadOnlySpan& slot)
{
	switch (code)
	{
	case XHt3000CmdReadInformation:
        {
			model = slot.GetByte(4);
			config = slot.GetByte(6);
			slot.GetString(8, 10, version, sizeof(version));
			checksum = slot.GetUInt32(18);
		}
		break;
	case XHt3000CmdReadSerial:
		{
			slot.GetString(10, 18, serial, sizeof(serial));
		}
		break;	
	case XHt3000CmdSetEthernet:
		{
			ip_address[0] = slot.GetByte(4);
			ip_address[1] = slot.GetByte(6);
			ip_address[2] = slot.GetByte(8);
			ip_address[3] = slot.GetByte(10);

			subnet_mask[0] = slot.GetByte(12);
			subnet_mask[1] = slot.GetByte(14);
			subnet_mask[2] = slot.GetByte(16);
			subnet_mask[3] = slot.GetByte(18);

			gateway[0] = slot.GetByte(20);
			gateway[1] = slot.GetByte(22);
			gateway[2] = slot.GetByte(24);
			gateway[3] = slot.GetByte(26);
		}
		break;
	default:
		return 0;
	}

	return slot.GetLength();
}

int XHt3000AsInform::Assemble(XHt3000PacketCommand code, std::string& slot)
{
	std::string s;	

	switch (code)
	{
	case XHt3000CmdReadInformation:
		{
			// AA BB CCCC DD EEEE FFFFFFFF
			s += fmt::format("{:02X}{:02X}", model, config);
			s += XHt3000AsPacketExtention::GetHex(version, sizeof(version) - 1);
			s += fmt::format("{:08X}", checksum);			
		}
		break;
	case XHt3000CmdReadSerial:
		{
			// AAAAAA BB CCCCCCCCCCCCCCCC
			s += fmt::format("{:06X}", 0);
			s += XHt3000AsPacketExtention::GetHex(serial, sizeof(serial) - 1);
		}
		break;
	case XHt3000CmdReadEthernet:
		{
            for (int i = 0; i < 4; i++) // A ,  8
                s += fmt::format("{:02X}", ip_address[i]);
			for (int i = 0; i < 4; i++) // B ,  8
				s += fmt::format("{:02X}", subnet_mask[i]);
			for (int i = 0; i < 4; i++) // C ,  8
				s += fmt::format("{:02X}", gateway[i]);
		}
		break;
	default:
		return 0;
	}

	slot = fmt::format("{:04X}{}", s.length() + 16, s);

	return (int)slot.length();
}

void XHt3000AsInform::PrintLog()
{
	LOGI << fmt::format(
		"XHt3000AsInform  model[{}] config[{}] version[{}] checksum[{}] serial[{}] "
		"set_ethernet[{}] ip_address[{}] subnet_mask[{}] gateway[{}]",
        model, config, version, checksum, serial,
        set_ethernet,
		fmt::format("{}.{}.{}.{}", ip_address[0], ip_address[1], ip_address[2], ip_address[3]),
        fmt::format("{}.{}.{}.{}", subnet_mask[0], subnet_mask[1], subnet_mask[2], subnet_mask[3]),
        fmt::format("{}.{}.{}.{}", gateway[0], gateway[1], gateway[2], gateway[3])
	);
}

//////////////////////////////////////////////////////////////////
bool Compare(const std::string s, char* buffer, int buffer_size)
{
	if (buffer_size != s.length())
		return false;
	for (int i = 0; i < s.length(); i++)
	{
		if (s[i] != buffer[i])
			return false;
	}

	return true;
}

int Test_XHt3000AsInformExample1()
{
	XHt3000AsInform inform;
	
	byte buffer[] = { 0x24, 0x30, 0x33, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x32, 0x36, 0x31, 0x45, 0x30, 0x31, 0x33, 0x30, 0x33, 0x32, 0x32, 0x45, 0x33, 0x30, 0x33, 0x36, 0x42, 0x30, 0x30, 0x37, 0x46, 0x30, 0x30, 0x46, 0x37, 0x34, 0x0D, };
	ReadOnlySpan slot(buffer, sizeof(buffer));
	XHt3000AsPacketExtention::PerformParse(&inform, slot);

	if (inform.model != 30)
		return 1;
	if (inform.config != 1)
		return 2;
	if (0 != strcmp(inform.version, "02.06"))
		return 3;
	if (inform.checksum != 2953310223)
		return 4;
	
	return 0;
}

int Test_XHt3000AsInformExample2()
{
	XHt3000AsInform inform1, inform2;
	inform1.model = 31;

	std::string default_ver = "02.06";
	inform1.model = 30;
	inform1.config = 1;
	strncpy_s(inform1.version, default_ver.c_str(), default_ver.length());
	inform1.checksum = 2953310223;
	
	byte buffer1[128] = { 0, };
	ReadOnlySpan slot(buffer1, 128);
	std::string s;
	inform1.Assemble(XHt3000CmdReadInformation, s);
    memcpy(buffer1, s.c_str(), s.length());
	inform2.Parse(XHt3000CmdReadInformation, slot);

	if (inform1.model != inform2.model)
		return 1;
	if (inform1.config != inform2.config)
		return 2;
	if (0 != strcmp(inform1.version, inform2.version))
		return 3;
	if (inform1.checksum != inform2.checksum)
		return 4;

	return 0;
}

int Test_XHt3000AsInformExample3()
{
	XHt3000AsInform inform;
	byte buffer1[128] = { 0x30, 0x30, 0x32, 0x38, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x34, 0x38, 0x33, 0x30, 0x33, 0x30, 0x33, 0x32, 0x33, 0x30, 0x33, 0x31, 0x33, 0x37, 0x33, 0x32, 0x33, 0x33, 0x30, 0x33, };
	ReadOnlySpan slot(buffer1, 128);
	inform.Parse(XHt3000CmdReadSerial, slot);

	if (0 != strcmp(inform.serial, "H00201723"))
		return 1;

	return 0;
}

int Test_XHt3000AsInformExample4()
{
	XHt3000AsInform inform1, inform2;

	std::string default_serial = "H00201723";
	strncpy_s(inform1.serial, default_serial.c_str(), default_serial.length());

	byte buffer1[128] = { 0, };
	std::string s;
	ReadOnlySpan slot(buffer1, 128);
	inform1.Assemble(XHt3000CmdReadSerial, s);
    memcpy(buffer1, s.c_str(), s.length());
	inform2.Parse(XHt3000CmdReadSerial, slot);

	if (0 != strcmp(inform1.serial, inform2.serial))
		return 1;

	return 0;
}

int Test_XHt3000AsInformExample5()
{
	XHt3000AsInform inform1, inform2;
	std::string default_ver = "02.06";
	inform1.model = 30;
	inform1.config = 1;
	strncpy_s(inform1.version, default_ver.c_str(), default_ver.length());
	inform1.checksum = 2953310223;	

	std::string s1;
    XHt3000AsPacketExtention::PerformAssemble(&inform1, XHt3000CmdReadInformation, s1);
	
	char buffer[] = { 0x24, 0x30, 0x33, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x32, 0x36, 0x31, 0x45, 0x30, 0x31, 0x33, 0x30, 0x33, 0x32, 0x32, 0x45, 0x33, 0x30, 0x33, 0x36, 0x42, 0x30, 0x30, 0x37, 0x46, 0x30, 0x30, 0x46, 0x37, 0x34, 0x0D };
	if (false == Compare(s1, buffer, sizeof(buffer)))
		return 1;    

	ReadOnlySpan slot((byte*)s1.c_str(), (int)s1.length());
	XHt3000AsPacketExtention::PerformParse(&inform2, slot);

	if (inform1.model != inform2.model)
		return 10;
	if (inform1.config != inform2.config)
		return 11;
	if (0 != strcmp(inform1.version, inform2.version))
		return 12;
	if (inform1.checksum != inform2.checksum)
		return 13;

	return 0;
}