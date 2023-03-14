#pragma once

int Echo(CPacketHeader* _header, unsigned short _size, std::shared_ptr<CGameUser>& _user)
{
	auto echoPacket = reinterpret_cast<CTestPacket*>(_header);
	printf("Packet Data : %s / Packet Size : %d \n", echoPacket->m_test, echoPacket->m_dataSize);

	auto sender = CSender::Alloc<CTestPacket>();
	auto packet = sender->GetWritePointer<CTestPacket>();
	packet->m_packetNum = 1;
	strcpy(packet->m_test, echoPacket->m_test);

	auto ptr = std::dynamic_pointer_cast<CTestUser>(_user);
	ptr->OnTest();

	return _user->Send(sender);
}

int BigPacketTest(CPacketHeader* _header, unsigned short _size, std::shared_ptr<CGameUser>& _user)
{
	auto testBigPacket = reinterpret_cast<CTestBigPacket*>(_header);
	printf("Packet Data : %s / Packet Size : %d \n", testBigPacket->m_bigData, testBigPacket->m_dataSize);

	auto sender = CSender::Alloc<CTestBigPacket>();
	auto packet = sender->GetWritePointer<CTestBigPacket>();
	packet->m_packetNum = 1;
	strcpy(packet->m_bigData, testBigPacket->m_bigData);

	return _user->Send(sender);
}