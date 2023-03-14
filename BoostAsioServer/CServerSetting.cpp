#include "CServerSetting.h"

CServerSetting::CServerSetting() : m_sessionCount(2000)
{

}

CServerSetting::~CServerSetting()
{

}

void CServerSetting::AddAddress(unsigned short _port)
{
	m_acceptorAddressList.emplace_back(nullptr, _port);
}

void CServerSetting::SetSessionCount(int _size)
{
	m_sessionCount = _size;
}

const AddressInfo& CServerSetting::GetAddress(int pIndex)
{
	assert(pIndex < GetAddressCount() && "index error!");

	return m_acceptorAddressList[pIndex];
}

unsigned short CServerSetting::GetAddressCount()
{
	return static_cast<unsigned short>(m_acceptorAddressList.size());
}

int CServerSetting::GetSessionCount()
{
	return m_sessionCount;
}