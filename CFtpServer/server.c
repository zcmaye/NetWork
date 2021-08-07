#include"../CFtp/CFtp.h"
#pragma comment(lib,"TcpSocket.lib")
#include<time.h>

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

		clock_t beg = clock();

		sendFile(clifd, info);

		printf("发送完毕，耗时%d\n", clock() - beg);
	}


	close_Socket();
	return 0;
}