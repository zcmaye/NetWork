#include"CHttp.h"
#pragma comment(lib,"TcpSocket.lib")

int main()
{
	init_Socket();
	SOCKET serfd = create_serverSocket(8080);
	printf("Server start succeed,wait connect...\n");

	while (true)
	{
		SOCKET clifd = accept(serfd, NULL, NULL);
		if (INVALID_SOCKET == clifd)
		{
			err("accept");
		}
		accept_request(clifd);
		//closesocket(clifd);
	}


	close_Socket();
	return 0;
}
