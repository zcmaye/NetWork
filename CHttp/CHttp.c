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
	char resPath[128] = "./zcMaye";		//请求的资源路径(网站主目录)
	char url[128] = { 0 };				//请求的资源url
	int method = getMothedAndUrl(fd, url, 128);
	switch (method)
	{
	case M_GET:
		//printf("GET:->%s\n", url);
		if (stricmp(url, "/") == 0)
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
			notFound(fd);
		}
		else
		{
			headers(fd, resPath);
			send_file(fd, resPath);
		}
		break;
	case M_POST:
		break;
	case M_PUT:
		break;
	default:
		break;
	}
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
	
	for (int k = 0; recvBuf[i]!=' ' && i < strlen(recvBuf); i++)
	{
		buf[k++] = recvBuf[i];
	}


	if (stricmp(method, "GET") == 0)
	{
		return M_GET;
	}
	else if (stricmp(method, "POST") == 0)
	{
		return M_POST;
	}
	else if (stricmp(method, "PUT") == 0)
	{
		return M_PUT;
	}
	return M_ERROR;
}

void notFound(SOCKET fd)
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

	send_file(fd,"./zcMaye/notFound404.html");
}

void send_file(SOCKET fd, const char* fileName)
{
	FILE* fp = fopen(fileName, "rb");
	if (!fp)
	{
		perror("open file failed");
		notFound(fd);
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
	strcpy(buf, "HTTP/1.0 200 OK\r\n");
	send(fd, buf, strlen(buf), 0);
	strcpy(buf, SERVER_STRING);
	send(fd, buf, strlen(buf), 0);
	sprintf(buf, "Content-Type: %s\r\n", getConentType(filename));
	send(fd, buf, strlen(buf), 0);
	sprintf(buf, "Content-length: %llu\r\n", getFileSize(filename));
	send(fd, buf, strlen(buf), 0);
	//sprintf(buf, "Transfer-Encoding: chunked\r\n");
	//send(fd, buf, strlen(buf), 0);
	strcpy(buf, "\r\n");
	send(fd, buf, strlen(buf), 0);
}

const char* getConentType(const char* fileName)
{
	//获取文件后缀
	char* beg = NULL;
	if ((beg = strrchr(fileName, '.')) == NULL)
	{
		printf("获取文件后缀失败~\n");
		return "text/unknow";
	}
	//常见的媒体格式类型
	if (stricmp(beg, ".html") == 0)
	{
		return "text/html";
	}
	else if (stricmp(beg, ".css") == 0)
	{
		return "text/css";
	}
	else if (stricmp(beg, ".xml") == 0)
	{
		return "text/xml";
	}
	else if (stricmp(beg, ".png") == 0)
	{
		return "image/apng";
	}
	else if (stricmp(beg, ".gif") == 0)
	{
		return "image/gif";
	}
	else if (stricmp(beg, ".jpg") == 0 || stricmp(beg, ".jpeg") == 0)
	{
		return "image/jpeg";
	}
	//以application开头的媒体格式
	else if (stricmp(beg, ".XHTML") == 0)
	{
		return "application/xhtml+xml";
	}
	else if (stricmp(beg, ".json") == 0)
	{
		return "application/json";
	}
	else if (stricmp(beg, ".pdf") == 0)
	{
		return "application/pdf";
	}
	// 二进制流数据（如常见的文件下载）
	else if (
		stricmp(beg, ".exe") == 0	||
		stricmp(beg, ".zip") == 0	||
		stricmp(beg, ".7z") == 0	||
		stricmp(beg, ".gz") == 0	||
		stricmp(beg, ".tar") == 0)
	{
		return "application/octet-stream";
	}
	return "text/plain";
}

size_t getFileSize(const char* fileName)
{
	struct _stat32i64 st = { 0 };
	if (0 != _stat32i64(fileName, &st))
	{
		perror("stat failed");
		return -1;
	}
	return (size_t)st.st_size;
}
