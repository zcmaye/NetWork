#pragma once
#include"../tcpSocket/tcpSocket.h"
#include<string.h>
#include<stdbool.h>
#define SEND_FILE_SIZE (100*1024*1024)	//每次发送的文件字节数

#ifdef _DEBUG
#define log(errMsg) printf("Line %d[%s] failed\n",__LINE__,errMsg)
#define print printf
#else
#define log(errMsg)
#define print 
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

struct CSmtp_info_;		//提前声明
/*@ 邮件所有信息结构提*/
typedef struct CSmtp_c
{
	//此成员仅供内部使用,程序员不要调用
	struct CSmtp_info_ * info;	
	//设置邮件标题和正文内容
	void (*setMailContent)(const char* title, const char* content);
	//设置端口号和域名
	void (*setPortDomain)(uint16 port, const char* domain);
	//发送邮件
	enum ErrNo (*sendEmal)();
	//添加附件
	void (*AddAttachment)(const char* filePath);
	//删除附件
	void (*DeleteAttachment)(const char* fileName);
}CSmtp;


//初始化邮件信息，用户名/密码/接收人邮箱
CSmtp* initSmtp(const char* userMain, const char* passwd, const char* toMail);
