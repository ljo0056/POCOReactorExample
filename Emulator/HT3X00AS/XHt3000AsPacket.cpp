#include "pch.h"
#include "XHt3000AsPacket.h"

ReadOnlySpan::ReadOnlySpan(const byte* data, int length)
	: m_data(data), m_length(length)
{
}

BYTE ReadOnlySpan::GetByte(int pos, int length) const
{
	char hexNum[8] = { 0, };
	memcpy(hexNum, &m_data[pos], length);
	return (BYTE)strtoul(hexNum, NULL, 16);
}

USHORT ReadOnlySpan::GetUShort(int pos, int length) const
{
	char hexNum[8] = { 0, };
	memcpy(hexNum, &m_data[pos], length);
	return (USHORT)strtoul(hexNum, NULL, 16);
}

UINT ReadOnlySpan::GetUInt32(int pos, int length) const
{
	char hexNum[8] = { 0, };
	memcpy(hexNum, &m_data[pos], length);
	return (UINT)strtoul(hexNum, NULL, 16);
}

INT ReadOnlySpan::GetInt32(int pos, int length) const
{
	char hexNum[8] = { 0, };
	memcpy(hexNum, &m_data[pos], length);
	return (INT)strtoul(hexNum, NULL, 16);
}

void ReadOnlySpan::GetString(int start, int length, char* str, int str_size) const
{
	memset(str, 0, str_size);
	if (0 != length % 2)
		return;

	char hexNum[8] = { 0, };
	int l = length / 2;
	for (int p = 0; p < l; p++)
	{
		memcpy(hexNum, m_data + (start + p * 2), 2);
		str[p] = (char)strtoul(hexNum, NULL, 16);
		memset(hexNum, 0, sizeof(hexNum));
	}
}

//////////////////////////////////////////////////////////////////

XHt3000AsPacket::XHt3000AsPacket()
{
    cmd = XHt3000CmdNone;
    memset(error, 0, sizeof(error));
    memset(status, 0, sizeof(status));
    length = 0;
    checksum = 0;
}

//////////////////////////////////////////////////////////////////

std::string XHt3000AsPacketExtention::GetHex(const char* str, int str_len)
{
    std::string hex_str;
    for (int i = 0; i < str_len; i++)
        hex_str += fmt::format("{:02X}", str[i]);
    return hex_str;
}

int XHt3000AsPacketExtention::PerformParse(XHt3000AsPacket* xht, const ReadOnlySpan& slot)
{    
	xht->cmd = (XHt3000PacketCommand)OperatorCode(slot); // or slot.GetByte(1);
	
    //xht->error[0] = slot.GetByte(3);
    //xht->error[1] = slot.GetByte(5);

    //xht->status[0] = slot.GetByte(7);
    //xht->status[1] = slot.GetByte(9);

    //xht->error1 = xht->error[0];
    //xht->error2 = xht->error[1];
    //xht->status1 = xht->status[0];
    //xht->status2 = xht->status[1];
	
    xht->length = PacketLength(slot); // or slot.GetUShort(11);
    if (slot.GetLength() != xht->length + 2) // STX + ETX
		return 0;
    
    xht->checksum = slot.GetByte(slot.GetLength() - 3);

	if (xht->length > 16)
	{
        // 14 = stx(1) + op(2) + error(4) + status(4) + checksum(2) + etx(1) = 1 + 2 + 4 + 4 + 2 + 1
		// Áï, length(4) + data °ª
		ReadOnlySpan need_parsing(slot.GetData() + 11, xht->length - 14);
		xht->Parse(xht->cmd, need_parsing);
	}

	return xht->length;
}

int XHt3000AsPacketExtention::PerformAssemble(XHt3000AsPacket* xht, XHt3000PacketCommand code, std::string& packet)
{		
	std::string data;
	if (xht->Assemble(code, data))
	{
		xht->error[0] = xht->error1;
		xht->error[1] = xht->error2;
		
		if (xht->cmd == XHt3000CmdReadSamplerEx2)
			xht->status[0] = xht->status1_new;
		else
			xht->status[0] = xht->status1;
		xht->status[1] = xht->status2;

		std::string s = fmt::format("{:02X}{:02X}{:02X}{:02X}{:02X}{}",
			(int)code, xht->error[0], xht->error[1], xht->status[0], xht->status[1], data);
		byte checksum = CalcChecksum(s);

		packet = fmt::format("{:c}{}{:02X}{:c}", STX, s, checksum, ETX);
		return (int)packet.length();
	}	

	return 0;
}

byte XHt3000AsPacketExtention::CalcChecksum(const std::string& data)
{
    return CalcChecksum((byte*)data.c_str(), (int)data.length());
}

byte XHt3000AsPacketExtention::CalcChecksum(byte* data, int lenght)
{
	byte checksum = data[0];
	for (int i = 1; i < lenght; i++)
		checksum ^= data[i];

	return checksum;
}

BYTE XHt3000AsPacketExtention::OperatorCode(const ReadOnlySpan& slot)
{
	return slot.GetByte(1);
}

USHORT XHt3000AsPacketExtention::PacketLength(const ReadOnlySpan& slot)
{
	return slot.GetUShort(11);
}

int XHt3000AsPacketExtention::RequestPacket(XHt3000PacketCommand code, std::string& packet)
{
	const int slot_size = 128;
	byte slot[slot_size] = { 0, };

	int length = 0;
	std::string s = fmt::format("{:02X}{:04X}{:04X}{:04X}", (int)code, 0, 0, 16);	

	byte checksum = CalcChecksum(s);
	packet = fmt::format("{:c}{}{:02X}{:c}", STX, s, checksum, ETX);

	return (int)packet.length();
}

//////////////////////////////////////////////////////////////////

int PacketExtention_RequestPacket_Test1()
{
	// Test Code by RequestPacket()	
	byte packet_inform[128] = { 0x24, 0x30, 0x33, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x31, 0x30, 0x30, 0x32, 0x0D, };
	byte buffer[128] = { 0, };
	std::string s1;
    XHt3000AsPacketExtention::RequestPacket(XHt3000CmdReadInformation, s1);
    memcpy(buffer, s1.c_str(), s1.length());

	int cmp = memcmp(buffer, packet_inform, 128);
	if (cmp != 0)
		return 1;

	memset(buffer, 0, 128);

	byte packet_serial[128] = { 0x24, 0x46, 0x46, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x31, 0x30, 0x30, 0x31, 0x0D, };
	std::string s2;
    XHt3000AsPacketExtention::RequestPacket(XHt3000CmdReadSerial, s2);
    memcpy(buffer, s2.c_str(), s2.length());

	cmp = memcmp(buffer, packet_serial, 128);
	if (cmp != 0)
		return 2;

	return 0;
}