#include "CHttp.h"
#include<sys/stat.h>
//请求方法
enum Method
{
	M_ERROR,
	M_GET,
	M_POST,
	M_PUT,
	M_HEAD,
	M_DELETE,
	M_PATCH,
	M_OPTIONS
};
void accept_request(SOCKET fd)
{
	char resPath[128] = "./zcMaye";		//请求的资源路径
	char url[128] = { 0 };			//请求的资源url
	int method = getMothedAndUrl(fd, url, 128);
	switch (method)
	{
	case M_GET:
		//printf("GET:->%s\n", url);
		if (strcmp(url, "/") == 0)
		{
			strcat_s(resPath, 128, "/index.html");
		}
		else
		{
			strcat_s(resPath, 128, url);
		}
		printf("resPath: %s\n", resPath);
		//查看文件是否存在
		struct stat st;
		if (stat(resPath, &st) == -1)
		{
			char tempBuf[1024] = { 0 };
			//把剩下的头部数据读出来丢掉
			//while (getline(fd, tempBuf, 1024))
			//{
			//	;
			//}
			//文件不存返回404
			not_found(fd);
		}
		else
		{
			headers(fd, resPath);
			send_file(fd, resPath);
		}
		break;
	case M_POST:
		break;
	default:
		break;
	}

	//closesocket(fd);
}

int getline(SOCKET fd, char* buf, int size)
{
	char c = '\0';
	int index = 0;

	while (index < size-1 && c!='\n')
	{
		int len = recv(fd, &c, 1, 0);
		if (len > 0)
		{
			buf[index++] = c;
		}
		else
		{
			//err("recv");
			break;
		}
	}
	return index;
}

int getMothedAndUrl(SOCKET fd, char* buf, int size)
{
	char recvBuf[1024] = { 0 };
	int len = getline(fd, recvBuf, 1024);
	//printf(" %s\n", recvBuf);

	char method[256] = { 0 };	//获取请求方法 GET POST ...
	int i = 0;
	for (; recvBuf[i] != ' ' && i < strlen(recvBuf); i++)
	{
		method[i] = recvBuf[i];
	}
	i++;//跳过空格
	printf("method:%s ", method);
	//获取Url
	int k = 0;
	for (; recvBuf[i]!=' ' && i < strlen(recvBuf); i++)
	{
		buf[k++] = recvBuf[i];
	}


	if (strcmp(method, "GET") == 0)
	{
		return M_GET;
	}
	else if (strcmp(method, "POST") == 0)
	{
		return M_POST;
	}
	return M_ERROR;
}

void not_found(SOCKET fd)
{
	char buf[1024];

	sprintf(buf, "HTTP/1.1 404 NOT FOUND\r\n");
	send(fd, buf, strlen(buf), 0);
	sprintf(buf, SERVER_STRING);
	send(fd, buf, strlen(buf), 0);
	sprintf(buf, "Content-Type: text/html\r\n");
	send(fd, buf, strlen(buf), 0);
	sprintf(buf, "\r\n");
	send(fd, buf, strlen(buf), 0);

	FILE* fp = fopen("./zcMaye/notFound404.html", "rb");
	if (!fp)
	{
		perror("open file failed");
		return;
	}
	while (!feof(fp))
	{
		char buf[1024] = { 0 };
		int len = fread(buf, sizeof(char), 1024, fp);
		send(fd, buf, len, 0);
	}
	fclose(fp);
}

void send_file(SOCKET fd, const char* fileName)
{
	FILE* fp = fopen(fileName, "rb");
	if (!fp)
	{
		perror("open file failed");
		not_found(fd);
		return;
	}
	while (!feof(fp))
	{
		char buf[1024] = { 0 };
		int len = fread(buf, sizeof(char), 1024, fp);
		send(fd, buf, len, 0);
	}
	fclose(fp);
}

void headers(SOCKET fd, const char* filename)
{
	char buf[1024];

	//获取文件后缀
	char* beg = NULL;
	if ((beg = strrchr(filename, '.'))==NULL)
	{
		printf("获取文件后缀失败~\n");
		return;
	}
	//确定文件类型
	char type[20] = { 0 };
	if (strcmp(beg, ".png") == 0)
	{
		strcpy_s(type,20,"image/apng");
	}
	else if (strcmp(beg, ".html") == 0)
	{
		strcpy_s(type, 20, "text/html");
	}
	else if (strcmp(beg, ".css") == 0)
	{
		strcpy_s(type, 20, "text/css");
	}

	strcpy(buf, "HTTP/1.0 200 OK\r\n");
	send(fd, buf, strlen(buf), 0);
	strcpy(buf, SERVER_STRING);
	send(fd, buf, strlen(buf), 0);
	sprintf(buf, "Content-Type: %s\r\n",type);
	send(fd, buf, strlen(buf), 0);
	strcpy(buf, "\r\n");
	send(fd, buf, strlen(buf), 0);
}
