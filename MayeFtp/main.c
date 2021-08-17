#include<stdio.h>
#include<tcpSocket.h>
#pragma comment(lib,"TcpSocket.lib")
#include"common.h"


int main()
{
	init_Socket();
	SOCKET serfd = create_serverSocket(21);
	printf("Ftp Server startup...\n");

	char recvBuf[1024] = { 0 };

	while (true)
	{
		printf("wait new Connect...\n");
		//接受客户端连接
		SOCKET clifd = accept(serfd, NULL, NULL);

		//处理客户端消息
		ClientInfo *clientInfo = createClientInfo();
		clientInfo->fd = clifd;

		welcome(clientInfo);
		while (true)
		{	
			memset(recvBuf, 0, sizeof(recvBuf));
			int len  = recv(clifd, recvBuf, 1024, 0);
			if (0 >= len)
			{
				break;
			}
			printf("main>%s",recvBuf);
			exeCmd(clientInfo,parseCmd(clientInfo,recvBuf));
		}
		closesocket(clifd);
	}
	close_Socket();
	return 0;
}