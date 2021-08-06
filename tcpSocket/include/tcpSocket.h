#ifndef _TCPSOCKET_H_
#define _TCPSOCKET_H_
#include<stdbool.h>
#include<stdio.h>
#include<WinSock2.h>				//头文件
#pragma comment(lib,"ws2_32.lib")	//库文件

#define err(errMsg) printf("[error] %s failed,code %d \
line:%d\n",errMsg, WSAGetLastError(),__LINE__);
#define PORT 8888	//0~1024 是系统保留，我们一般不用

typedef unsigned short uint16;

//初始化网络库
bool init_Socket();
//关闭网络库
bool close_Socket();
//服务器：创建服务器socket
SOCKET create_serverSocket();
//客户端：创建客户端socket
SOCKET create_clientSocket(const char* ip,uint16 port);

#endif // !_TCPSOCKET_H_
