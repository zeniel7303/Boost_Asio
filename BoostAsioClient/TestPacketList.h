#pragma once

int Echo(CPacketHeader* _header, unsigned short _size)
{
	auto echoPacket = reinterpret_cast<CTestPacket*>(_header);
	printf("Packet Data : %s / Packet Size : %d \n", echoPacket->m_test, echoPacket->m_dataSize);

	auto sender = CSender::Alloc<CTestPacket>();
	auto packet = sender->GetWritePointer<CTestPacket>();
	packet->m_packetNum = 1;
	strcpy(packet->m_test, echoPacket->m_test);

	return 0;
}

int BigPacketTest(CPacketHeader* _header, unsigned short _size)
{
	auto testBigPacket = reinterpret_cast<CTestBigPacket*>(_header);

	printf("Packet Data : %d \n", testBigPacket->m_dataSize);

	return 0;
}