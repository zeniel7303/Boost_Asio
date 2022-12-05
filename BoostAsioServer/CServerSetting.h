#include "CServer.h"

struct AddressInfo
{
	char*			m_ip;
	unsigned short	m_port;

	AddressInfo(char* _ip, unsigned short _port)
	{
		m_ip = _ip;
		m_port = _port;
	}
};

class CServerSetting
{
protected:
	std::vector<AddressInfo>	m_acceptorAddressList;
	int							m_sessionCount;

public:
	CServerSetting();
	~CServerSetting();

	void AddAddress(char* _ip, unsigned short _port);
	void AddAddress(unsigned short _port);
	void SetSessionCount(int _size);

	const AddressInfo& GetAddress(int pIndex);
	unsigned short GetAddressCount();
	int GetSessionCount();
};