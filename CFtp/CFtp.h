#pragma once
#include<tcpSocket.h>

typedef struct FileInfo_
{
	size_t size;	//文件大小
	char name[128];	//文件名
}FileInfo;

const char* getFileName(const char* filePaht);
size_t getFileSize(const fileName);


/***服务器*****/
//从客户端接受文件大小和文件名
FileInfo recvFileName(SOCKET fd);
//发送文件
bool sendFile(SOCKET fd,FileInfo fileInfo);

/***客户端****/
//向服务器发送文件大小和文件名
bool sendFileName(SOCKET fd,const char* fileName);
//接收文件,fileName 为保存的文件名
bool recvFile(SOCKET fd,const char* fileName);
