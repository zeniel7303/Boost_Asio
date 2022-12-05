#include "CTcpClient.h"

int main()
{
	CTcpClient client;
	client.Connect("127.0.0.1", 30001);

	for (;;)
	{	
		std::cout << "Enter Message : ";
		/*char message[256];
		std::cin.getline(message, 256);
		if (strcmp(message, "quit") == 0)
		{
			break;
		}*/

		char message[256] = "Test Message";

		client.TestFunc(message);
		client.Receive();
		client.ProcessPacket();

		std::cout << " ================================================== " << std::endl;
	
		Sleep(100);
	}

	return 0;
}