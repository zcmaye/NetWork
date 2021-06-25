#include "CSmtp_c.h"

#define built_in_function static	//内部函数，禁止在外部使用
#define built_in_struct 	//内部结构体，禁止在外部使用

const char* base64Encode(const char* origSigned, size_t origLength);
built_in_function void FormatEmailHead(char* str);
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
//发送附件
enum ErrNo sendAttachment();


/*@发送附件使用的文件结构体*/
typedef struct
{
	char filePath[128];
	char fileName[128];
}FileInfo;
/*@顺序表*/
typedef struct
{
	size_t size;
	FileInfo* base;
}SeqList;
SeqList* g_list;
built_in_function SeqList* createSeqList()
{
	SeqList* list = calloc(1, sizeof(SeqList));
	if (!list)
		return NULL;
	list->base = calloc(8, sizeof(FileInfo));
	if (!list->base)
	{
		free(list);
		return NULL;
	}
	return list;
}
built_in_function void push(FileInfo fileinfo)
{
	g_list->base[g_list->size++] = fileinfo;
}
built_in_function void erase(FileInfo fileinfo)
{
	for (size_t i = 0; i < g_list->size; i++)
	{
		if (strcmp(g_list->base[i].fileName, fileinfo.fileName) == 0)
		{
			for (size_t pos = i; pos < g_list->size - 1; pos++)
			{
				g_list->base[pos] = g_list->base[pos + 1];
			}
			g_list->size--;
			return;
		}
	}
}
typedef FileInfo* Iterator;
built_in_function Iterator begin()
{
	return &g_list->base[0];
}
built_in_function Iterator end()
{
	return &g_list->base[g_list->size];
}

built_in_struct typedef struct CSmtp_info_
{
	char	userMail[30];			//用户邮箱
	char	passwd[20];				//用户密码
	char	toMail[30];				//接收人邮箱
	char	title[30];				//邮件标题
	char* contemt;				//邮件内容
	//服务器相关
	uint16	port;					//端口号
	char	domain[20];				//服务器域名
	SOCKET	fd;						//描述符
	char	recvBuf[BUFSIZ + 1];	//接受数据的缓冲区
}CSmtp_info;
CSmtp g_csmtp;
//初始化成员函数
void initFunctionPointer()
{
	g_csmtp.AddAttachment = AddAttachment;
	g_csmtp.sendEmal = sendEmal;
	g_csmtp.setMailContent = setMailContent;
	g_csmtp.setPortDomain = setPortDomain;
	g_csmtp.DeleteAttachment = DeleteAttachment;
}

//初始化邮件信息，用户名/密码/接收人邮箱
CSmtp* initSmtp(const char* userMain, const char* passwd, const char* toMail)
{
	g_csmtp.info = calloc(1, sizeof(CSmtp_info));
	if (!g_csmtp.info)
	{
		print("line %d:initSmtp info init error\n", __LINE__);
		return NULL;
	}
	strcpy(g_csmtp.info->userMail, userMain);
	strcpy(g_csmtp.info->passwd, passwd);
	strcpy(g_csmtp.info->toMail, toMail);

	g_csmtp.info->port = 25;
	strcpy(g_csmtp.info->domain, "smtp.qq.com");
	g_csmtp.info->fd = 0;
	memset(g_csmtp.info->recvBuf, 0, BUFSIZ + 1);

	g_list = createSeqList();

	initFunctionPointer();
	return &g_csmtp;
}
//设置邮件标题和正文内容
void setMailContent(const char* title, const char* content)
{
	strcpy(g_csmtp.info->title, title);
	if (content != NULL && g_csmtp.info->contemt == NULL)
	{
		int len = strlen(content);
		g_csmtp.info->contemt = calloc(len + 1, sizeof(char));
		if (!g_csmtp.info->contemt)
		{
			print("line %d: setMailContent memory failed\n", __LINE__);
			return;
		}
		strcpy(g_csmtp.info->contemt, content);
	}
}
//设置端口号和域名
void setPortDomain(uint16 port, const char* domain)
{
	g_csmtp.info->port = port;
	strcpy(g_csmtp.info->domain, domain);
}

//从socket接受消息
built_in_function bool recvMsg()
{
	int ret = recv(g_csmtp.info->fd, g_csmtp.info->recvBuf, BUFSIZ, 0);
	if (ret == SOCKET_ERROR)
	{
		print("line %d:recvMsg recv error code (%d)\n", __LINE__,WSAGetLastError());
		return false;
	}
	g_csmtp.info->recvBuf[ret] = '\0';
	printf("recv> %s\n", g_csmtp.info->recvBuf);
	return true;
}
//向socket发送消息
built_in_function bool sendMsg(const char* msg)
{
	if (SOCKET_ERROR == send(g_csmtp.info->fd, msg, strlen(msg), 0))
	{
		print("line %d:sendMsg send error code (%d)\n", __LINE__, WSAGetLastError());
		return false;
	}
	printf("send> %s\n", msg);
	return true;
}
//连接到服务器
built_in_function bool connectToHost()
{
	init_Socket();		//初始化socket库

	//根据域名获取主机的信息
	struct hostent* hostInfo = gethostbyname(g_csmtp.info->domain);

	//const char* tdomain = inet_ntoa(*(struct in_addr*)&hostInfo->h_addr_list[0]);
	//DWORD  ipnum = *(DWORD*)hostInfo->h_addr_list[0];
	//printf("%s\n", hostInfo->h_addr_list[0]);
	//printf("%s %d\n", tdomain, ipnum);
	//printf("%s %d\n", tdomain, inet_addr(tdomain));

	//把二进制ip地址转成点分十进制
	char hostIp[50] = { 0 };
	inet_ntop(AF_INET, hostInfo->h_addr_list[0], hostIp, 50);

	//连接到服务器
	g_csmtp.info->fd = create_clientSocket(hostIp, g_csmtp.info->port);
	if (g_csmtp.info->fd == INVALID_SOCKET)
	{
		print("line %d:connectToHost SOCKET error code (%d)\n", __LINE__, WSAGetLastError());
		return false;
	}
	return recvMsg();
}

//登录
built_in_function enum ErrNo login()
{
	char sendBuf[BUFSIZ] = { 0 };
	sprintf(sendBuf, "EHLO %s \r\n", g_csmtp.info->userMail);
	if (!sendMsg(sendBuf) || !recvMsg())
	{
		return NetWorkError;
	}

	sprintf(sendBuf, "AUTH LOGIN\r\n%c", '\0');
	if (!sendMsg(sendBuf) || !recvMsg())
	{
		return NetWorkError;
	}

	//发送账号
	char* encode = base64Encode(g_csmtp.info->userMail, strlen(g_csmtp.info->userMail));
	sprintf(sendBuf, "%s\r\n%c", encode, '\0');
	free(encode);
	if (!sendMsg(sendBuf) || !recvMsg())
	{
		return NetWorkError;
	}

	//发送密码
	encode = base64Encode(g_csmtp.info->passwd, strlen(g_csmtp.info->passwd));
	sprintf(sendBuf, "%s\r\n%c", encode, '\0');
	free(encode);
	if (!sendMsg(sendBuf) || !recvMsg())
	{
		return NetWorkError;
	}

	//检查是否登录成功
	if (strstr(g_csmtp.info->recvBuf, "550"))
	{
		print("line %d:login user error\n", __LINE__);
		return UserNameError;
	}
	if (strstr(g_csmtp.info->recvBuf, "535"))
	{
		print("line %d:login passwd error\n", __LINE__);
		return UserPassError;
	}
	return NoError;
}
//发送邮件头部信息
built_in_function bool sendEmailHead()
{
	char sendBuf[BUFSIZ] = { 0 };
	sprintf(sendBuf, "MAIL FROM:<%s>\r\n%c", g_csmtp.info->userMail, '\0');
	if (!sendMsg(sendBuf) || !recvMsg())
	{
		return false;
	}

	sprintf(sendBuf, "RCPT TO:<%s>\r\n%c", g_csmtp.info->toMail, '\0');
	if (!sendMsg(sendBuf) || !recvMsg())
	{
		return false;
	}

	sprintf(sendBuf, "DATA\r\n%c", '\0');
	if (!sendMsg(sendBuf) || !recvMsg())
	{
		return false;
	}

	//发送完头部之后不必调用接收函数,因为你没有\r\n.\r\n结尾，服务器认为你没有发完数据，所以不会返回什么值
	FormatEmailHead(sendBuf);
	if (!sendMsg(sendBuf))
	{
		return false;
	}
	return true;
}
//格式化邮件头部
built_in_function void FormatEmailHead(char* str)
{
	memset(str, 0, BUFSIZ);

	char tstr[128] = { 0 };
	sprintf(tstr, "From: %s\r\n%c", g_csmtp.info->userMail, '\0');
	strcat(str, tstr);

	sprintf(tstr, "To: %s\r\n%c", g_csmtp.info->toMail, '\0');
	strcat(str, tstr);

	sprintf(tstr, "Subject: %s\r\n%c", g_csmtp.info->title, '\0');
	strcat(str, tstr);

	sprintf(tstr, "MINE-Version: 1.0\r\n%c", '\0');
	strcat(str, tstr);

	sprintf(tstr, "Content-Type:multipart/mixed;boundary=maye\r\n%c", '\0');
	strcat(str, tstr);

	sprintf(tstr, "\r\n%c", '\0');
	strcat(str, tstr);
}
//发送正文
built_in_function bool sendTextbody()
{
	char sendBuf[BUFSIZ] = { 0 };
	strcat(sendBuf, "--maye\r\n");
	strcat(sendBuf, "Content-Type:text/plain;charset=\"gb2312\"\r\n\r\n");
	strcat(sendBuf, g_csmtp.info->contemt);
	strcat(sendBuf, "\r\n\r\n");
	return sendMsg(sendBuf);
}
//拜拜
built_in_function bool sendEnd() /*发送结尾信息*/
{
	char sendBuf[BUFSIZ] = { 0 };
	strcat(sendBuf, "--maye--\r\n.\r\n");
	if (false == sendMsg(sendBuf) || false == recvMsg())
	{
		return false;
	}
	printf("%s\b", sendBuf);

	sprintf(sendBuf, "QUIT\r\n%c", '\0');
	return (sendMsg(sendBuf) && recvMsg());
}

//发送邮件
enum ErrNo sendEmal()
{
	if (!connectToHost())
	{
		return NetWorkError;
	}

	enum ErrorNo ret = login();
	if (ret != NoError)
	{
		return ret;
	}

	if (!sendEmailHead())
	{
		return NetWorkError;
	}

	if (!sendTextbody())
	{
		return NetWorkError;
	}

	sendAttachment();

	if (!sendEnd())
	{
		return NetWorkError;
	}
	return NoError;
}
//添加附件
void AddAttachment(const char* filePath)
{
	FileInfo info;
	strcpy(info.filePath, filePath);
	char* str = strrchr(filePath, '/');
	if (str != NULL)
	{
		strcpy(info.fileName, str + 1);
		push(info);
		return;
	}

	str = strrchr(filePath, '\\');
	if (str != NULL)
	{
		strcpy(info.fileName, str + 1);
		push(info);
		return;
	}
	else
	{
		//在当前文件夹下，而且没有写./
		strcpy(info.fileName, filePath);
		push(info);
		return;
	}	
}
//删除附件
void DeleteAttachment(const char* fileName)
{
	FileInfo info;
	strcpy(info.fileName, fileName);
	erase(info);
}
//发送附件
enum ErrNo sendAttachment()
{
	printf("begin send attachment ~~~~\n");
	for (Iterator it = begin(); it != end(); it++)
	{
		FILE* fp = fopen(it->filePath, "rb");
		if (!fp)
		{
			print("%s:文件打开失败~\n", it->filePath);
			return FileOpenError;
		}

		char sendBuf[BUFSIZ] = { 0 };
		char tempbuf[100] = { 0 };
		//发送文件头部信息
		strcat(sendBuf, "--maye\r\n");
		strcat(sendBuf, "Content-Type:application/octet-stream;\r\n");
		sprintf(tempbuf, " name=\"%s\"\r\n", it->fileName);//注意name前面必须要有空格（不知道啥原因）
		strcat(sendBuf, tempbuf);

		strcat(sendBuf, "Content-Transfer-Encoding: base64\r\n");
		strcat(sendBuf, "Content-Disposition: attachment;\r\n");
		sprintf(tempbuf, "fileNmae=\"%s\"\r\n", it->fileName);
		strcat(sendBuf, tempbuf);
		strcat(sendBuf, "\r\n");
		sendMsg(sendBuf);

		//读取文件，加密并传送
		char* fileBuf = (char*)calloc(SEND_FILE_SIZE + 1, sizeof(char));
		if (fileBuf == NULL)
		{
			print("line %d:sendAttachment MenoryAllocError\n", __LINE__);
			return MenoryAllocError;
		}
		char* encode = NULL;
		while (!feof(fp))
		{
			int size = fread(fileBuf, sizeof(char), SEND_FILE_SIZE, fp);
			fileBuf[size] = '\0';
			//encode = base64Encode(fileBuf, strlen(fileBuf));		//此种方法发送二进制文件时会出现问题
			encode = base64Encode(fileBuf, size);
			encode[strlen(encode)] = '\r';
			encode[strlen(encode)] = '\n';
			//sendMsg(encode);
			send(g_csmtp.info->fd, encode, strlen(encode), 0);
			free(encode);
		}

		free(fileBuf);
		fclose(fp);
	}
	return NoError;
}




static const char base64Char[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
const char* base64Encode(const char* origSigned, size_t origLength)
{
	unsigned char const* orig = (unsigned char const*)origSigned; // in case any input bytes have the MSB set
	if (orig == NULL) return NULL;

	unsigned const numOrig24BitValues = origLength / 3;
	bool havePadding = origLength > numOrig24BitValues * 3;
	bool havePadding2 = origLength == numOrig24BitValues * 3 + 2;
	unsigned const numResultBytes = 4 * (numOrig24BitValues + havePadding);
	//多申请三个字节大小，可以放\r\n\0
	char* result = calloc(numResultBytes + 3, sizeof(char)); // allow for trailing '/0'

												 // Map each full group of 3 input bytes into 4 output base-64 characters:
	unsigned i;
	for (i = 0; i < numOrig24BitValues; ++i)
	{
		result[4 * i + 0] = base64Char[(orig[3 * i] >> 2) & 0x3F];
		result[4 * i + 1] = base64Char[(((orig[3 * i] & 0x3) << 4) | (orig[3 * i + 1] >> 4)) & 0x3F];
		result[4 * i + 2] = base64Char[((orig[3 * i + 1] << 2) | (orig[3 * i + 2] >> 6)) & 0x3F];
		result[4 * i + 3] = base64Char[orig[3 * i + 2] & 0x3F];
	}

	// Now, take padding into account.  (Note: i == numOrig24BitValues)
	if (havePadding)
	{
		result[4 * i + 0] = base64Char[(orig[3 * i] >> 2) & 0x3F];
		if (havePadding2)
		{
			result[4 * i + 1] = base64Char[(((orig[3 * i] & 0x3) << 4) | (orig[3 * i + 1] >> 4)) & 0x3F];
			result[4 * i + 2] = base64Char[(orig[3 * i + 1] << 2) & 0x3F];
		}
		else
		{
			result[4 * i + 1] = base64Char[((orig[3 * i] & 0x3) << 4) & 0x3F];
			result[4 * i + 2] = '=';
		}
		result[4 * i + 3] = '=';
	}

	result[numResultBytes] = '\0';
	return result;
}