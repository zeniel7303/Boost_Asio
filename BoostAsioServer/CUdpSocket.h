#include "CServer.h"
#include "CIOContext.h"

class CUdpSocket
{
private:
	boost::asio::ip::udp::socket m_socket;

public:
	CUdpSocket();
	~CUdpSocket();

	int Receive(void* _buffer, int _size, int& _receivedSize, boost::asio::ip::udp::endpoint& _endPoint);
	int Send(void* _buffer, int _size, int& _sendSize, boost::asio::ip::udp::endpoint& _endPoint);
	void SetReuse(bool _isReuse);
	int Close();

	boost::asio::ip::udp::socket& GetSocket() { return m_socket; }
	const boost::asio::ip::udp::endpoint&& GetEndPoint() const { return m_socket.local_endpoint(); }

	//async
	template<typename T>
	void ReceiveAsync(void* _buffer, int _size, boost::asio::ip::udp::endpoint& _endPoint, T&& _t);
	template<typename T>
	void SendAsync(void* _buffer, int _size, boost::asio::ip::udp::endpoint& _endPoint, T&& _t);
};

#include "CUdpSocket.Inl"