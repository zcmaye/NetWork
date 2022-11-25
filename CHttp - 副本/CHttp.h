#pragma once
#include"tcpSocket.h"
#define SERVER_STRING "Server: zcMaye/0.1.0\r\n"

void accept_request(SOCKET fd);

//获取一行数据(数据以\r\n结尾)
int readLine(SOCKET fd,char*buf,int size);
//获取方法和请求的连接
int getMothedAndUrl(SOCKET fd,char*buf,int size);
//404
void notFound(SOCKET client);
//发送文件
void send_file(SOCKET fd, const char* fileName);
//响应头部信息
void headers(SOCKET fd, const char* filename);
//确定文件类型
const char* getConentType(const char* fileName);
//获取文件大小
size_t getFileSize(const char* fileName);