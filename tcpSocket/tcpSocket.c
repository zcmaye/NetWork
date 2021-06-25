#include"tcpSocket.h"
#include<WS2tcpip.h>
bool init_Socket()
{
	WSADATA wsadata;
	if (0 != WSAStartup(MAKEWORD(2, 2), &wsadata))		//wsa  windows socket ansyc  windows异步套接字
	{
		err("WSAStartup");
		return false;
	}
	return true;
}

bool close_Socket()
{
	if (0 != WSACleanup())
	{
		err("WSACleanup");
		return false;
	}
	return true;
}

SOCKET create_serverSocket()
{
	//1，创建一个空的socket
	SOCKET fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == fd)
	{
		err("socket");
		return INVALID_SOCKET;
	}
	//~0  对于有符号来说是-1  对于无符号来说是最大值

	//2，给socket绑定本地ip地址和端口号
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(PORT);			//把本地字节序转为网络字节序， 大端存储和小端存储
	addr.sin_addr.S_un.S_addr = ADDR_ANY;	//绑定本地任意ip
	
	if (SOCKET_ERROR == bind(fd, (struct sockaddr*)&addr, sizeof(addr)))
	{
		err("bind");
		return INVALID_SOCKET;
	}

	//2，开始监听
	listen(fd, 10);

	return fd;
}

SOCKET create_clientSocket(const char* ip,uint16 port)
{
	//1，创建一个空的socket
	SOCKET fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == fd)
	{
		err("socket");
		return INVALID_SOCKET;
	}
	//~0  对于有符号来说是-1  对于无符号来说是最大值

	//2，给socket绑定服务端的ip地址和端口号
	//struct sockaddr;
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);			//把本地字节序转为网络字节序， 大端存储和小端存储
	//addr.sin_addr.S_un.S_addr = *((DWORD*)ip);			//绑定服务器ip
	
	inet_pton(AF_INET, ip, &addr.sin_addr);


	if (INVALID_SOCKET == connect(fd, (struct sockaddr*)&addr, sizeof(addr)))
	{
		err("connet");
		return INVALID_SOCKET;
	}
	return fd;
}


//文件操作
int g_fileSzie;	//保存文件大小
char* g_fileBuf;			//保存文件数据

bool readFile(const char* fileName)
{
	FILE* read = fopen(fileName, "rb");
	if (!read)
	{
		perror("file open failed");
		return false;
	}
	//获取文件大小
	fseek(read, 0, SEEK_END);	//将文件位置指针移动到最后
	g_fileSzie = ftell(read);
	fseek(read, 0, SEEK_SET);	//移动到开头
	printf("filesize :%d\n", g_fileSzie);

	//分配内存
	g_fileBuf = calloc(g_fileSzie, sizeof(char));
	if (!g_fileBuf)
		return false;

	//把文件读到内存中来
	fread(g_fileBuf, sizeof(char), g_fileSzie, read);

	fclose(read);
	return true;
}

bool sendFile(SOCKET s, const char* fileName)
{
	//文件读取失败直接退出
	if (readFile(fileName) == false)
	{
		return false;
	}

	//发送文件大小
	send(s, &g_fileSzie, sizeof(int), 0);
	//发送文件内容
	int ret = send(s, g_fileBuf, g_fileSzie, 0);
	if (ret == SOCKET_ERROR)
	{
		err("sendFile");
		return false;
	}
	printf("文件发送成功,共(%d)Byte\n", ret);

	return false;
}

bool recvFile(SOCKET s, const char* fileName)
{
	if (g_fileBuf == NULL)
	{
		
		recv(s, &g_fileSzie, sizeof(int), 0);
		printf("recv filesize:%d\n", g_fileSzie);
		g_fileBuf = calloc(g_fileSzie, sizeof(char));
		if (!g_fileBuf)
			return false;
	}

	int ret = recv(s, g_fileBuf, g_fileSzie, 0);
	if (ret == 0)
	{
		printf("服务器正常下线...\n");
	}
	else if (ret < 0)
	{
		err("recv");
	}

	saveFile(fileName);

	return false;
}

bool saveFile(const char* fileName)
{
	FILE* write = fopen(fileName, "wb");
	if (!write)
	{
		perror("file open failed : \n");
		return false;
	}

	fwrite(g_fileBuf, sizeof(char), g_fileSzie, write);

	fclose(write);
	return true;
}