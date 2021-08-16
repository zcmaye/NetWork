#include<stdio.h>
#include<tcpSocket.h>
#pragma comment(lib,"TcpSocket.lib")
#include"common.h"
int main()
{
	init_Socket();
	SOCKET serfd = create_serverSocket(8848);
	char recvBuf[1024] = { 0 };
	SOCKET clifd = accept(serfd, NULL, NULL);
	printf("have new connect\n");


	ClientInfo clientInfo;
	clientInfo.fd = clifd;
	welcome(&clientInfo);
	while (true)
	{	
		memset(recvBuf, 0, sizeof(recvBuf));
		recv(clifd, recvBuf, 1024, 0);
		printf("main>%s",recvBuf);
		exeCmd(&clientInfo,parseCmd(recvBuf));

	}
	close_Socket();
	return 0;
}