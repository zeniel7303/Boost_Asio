#pragma once
#include "CServer.h"
#include "CIOContext.h"

class CTcpSocket
{
private:
	boost::asio::ip::tcp::socket m_socket;

public:
	CTcpSocket();
	~CTcpSocket();

	//async
	template<typename T>
	int ConnectAsync(const char* _ip, unsigned short _port, T&& _t);
	template<typename T>
	void ReceiveAsync(void* _buffer, int _size, T&& _t);
	template<typename T>
	void SendAsync(void* _buffer, int _size, T&& _t);

	const boost::asio::ip::tcp::socket& GetSocket() const { return m_socket; }
	boost::asio::ip::tcp::socket& GetSocket() { return m_socket; }
	const boost::asio::ip::tcp::endpoint GetEndPoint() const { return m_socket.local_endpoint(); }

	int Connect(const char* _ip, unsigned short _port);
	int Receive(void* _buffer, int _size, int& _receivedSize);
	int Send(void* _buffer, int _size, int& _sendSize);
	void SetReuse(bool _isReuse);
	int Close();
};

#include "CTcpSocket.Inl"