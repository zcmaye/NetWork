#include"../CFtp/CFtp.h"
#pragma comment(lib,"TcpSocket.lib")
#include<time.h>
int main()
{
	init_Socket();

	SOCKET fd = create_clientSocket("127.0.0.1", 8888);

	char fileName[128] = { 0 };
	printf("download fileName>");
	gets_s(fileName, 128);
	sendFileName(fd, fileName);

	clock_t beg = clock();

	recvFile(fd, getFileName(fileName));

	printf("接受完毕，耗时：%lu\n",clock() -beg);
	getchar();
	closesocket(fd);
	close_Socket();
	return 0;
}