#pragma once
#include"tcpSocket.h"
#pragma comment(lib,"TcpSocket.lib")

#include<WS2tcpip.h>
#include<string.h>
#include<stdbool.h>
#define SEND_FILE_SIZE (100*1024*1024)	//每次发送的文件字节数

#ifdef _DEBUG
#define log(errMsg) printf("Line %d[%s] failed\n",__LINE__,errMsg)
#else
#define log(errMsg) 
#endif

/*@错误类型*/
enum ErrNo
{
	NoError,
	NetWorkError,		//网络错误
	UserNameError,		//用户名错误
	UserPassError,		//密码错误
	FileOpenError,
	MenoryAllocError,	//内存分配失败
};

/*@发送附件使用的文件结构体*/
typedef struct
{
	char filePath[128];
	char fileName[128];
}FileInfo;



//初始化邮件信息，用户名/密码/接收人邮箱
void initSmtp(const char* userMain, const char* passwd, const char* toMail);
//设置邮件标题和正文内容
void setMailContent(const char* title, const char* content);
//设置端口号和域名
void setPortDomain(uint16 port, const char* domain);
//发送邮件
enum ErrNo sendEmal();
//添加附件
void AddAttachment(const char* filePath);
//删除附件
void DeleteAttachment(const char* fileName);