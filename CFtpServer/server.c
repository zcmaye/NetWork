#include"../CFtp/CFtp.h"
#pragma comment(lib,"TcpSocket.lib")

int main()
{
	init_Socket();

	SOCKET serfd = create_serverSocket();
	
	while (true)
	{
		printf("server start succeed,wait connet...\n");
		SOCKET clifd = accept(serfd, NULL, NULL);
		if (clifd == INVALID_SOCKET)
		{
			err("accept");
			continue;
		}
		FileInfo info = recvFileName(clifd);
		sendFile(clifd, info);

		printf("size:%d name:%s\n\n", info.size, info.name);
	}


	close_Socket();
	return 0;
}