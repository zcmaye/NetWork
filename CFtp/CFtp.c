#include "CFtp.h"
#include<sys/stat.h>
#define TRANSFER_SIZE (100*1024*1024)	//每次传送的字节大小 100M
//文件操作
const char* getFileName(const char* filePath)
{
	const char* beg = NULL;
	//如果文件名包含路径，把文件名拆出来
	if ((beg = strrchr(filePath, '/')) || (beg = strrchr(filePath, '\\')))
	{
		beg++;
	}
	//不包含路径，就直接是文件名了
	else
	{
		beg = filePath;
	}
	return beg;
}

size_t getFileSize_self(const char* fileName)
{
	FILE* fp = fopen(fileName,"rb");
	if (!fp)
	{
		perror("file open failed");
		return 0;
	}
	char* buf = malloc(100 * 1024 * 1024);
	if (!buf)
	{
		printf("getfileSize memory failed\n");
		return false;
	}
	size_t totalSize = 0;
	while (!feof(fp))
	{
		totalSize += fread(buf, sizeof(char), 100 * 1024 * 1024, fp);
	}
	free(buf);
	fclose(fp);
	return totalSize;
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

FileInfo recvFileName(SOCKET fd)
{
	FileInfo info = { 0 };
	//获取文件名长度
	size_t fileNameSize = 0;
	if (0 >= recv(fd, (char*)&fileNameSize, sizeof(size_t), 0))
	{
		err("recv");
		return (FileInfo) { 0, "NULL" };
	}
	//获取文件名 sizeof(info.name)-1 减1是为了给\0腾位置
	if (0 >= recv(fd, info.name, sizeof(info.name) - 1, 0))
	{
		err("recv");
		return (FileInfo) { 0, "NULL" };
	}

	//拼接文件路径，默认在files下面
	char path[128] = { 0 };
	strcat(path, "./files/");
	strcat(path, info.name);
	strcpy(info.name, path);

	//获取文件大小
	if ((info.size = getFileSize(info.name)) == 0)
	{
		return (FileInfo) { 0, "NULL" };
	}	
	printf("fileSize:%llu fileName:%s\n", info.size, info.name);
	return info;
}

bool sendFile(SOCKET fd, FileInfo fileInfo)
{
	//发送文件大小给客户端
	if (SOCKET_ERROR == send(fd, &fileInfo.size, sizeof(size_t), 0))
	{
		err("send");
		return false;
	}

	//读取文件并发送
	FILE* fp = fopen(fileInfo.name, "rb");
	if (!fp)
	{
		perror("open file faile");
		return false;
	}
	char* buf = calloc(TRANSFER_SIZE, sizeof(char));
	if (!buf)
	{
		printf("Menory failed\n");
		return false;
	}
	printf("文件发送中。。。\n");
	size_t sendSize = 0;
	for (size_t i = 0; sendSize < fileInfo.size; i++)
	{
		int ret = fread(buf, sizeof(char), TRANSFER_SIZE, fp);
		if (SOCKET_ERROR == send(fd, buf, ret, 0))
		{
			err("send");
			return false;
		}
		sendSize += ret;
	}
	free(buf);
	fclose(fp);
	return true;
}


bool sendFileName(SOCKET fd, const char* fileName)
{
	if (!fileName)
		return false;

	//获取文件名（不带路径的）
	char* beg = getFileName(fileName);
	//文件名长度
	size_t fileNameSize = strlen(beg);

	if (SOCKET_ERROR == send(fd, (char*)&fileNameSize, sizeof(size_t), 0) ||
		SOCKET_ERROR == send(fd, beg, strlen(beg), 0))
	{
		err("sendFileName failed\n");
		return false;
	}
	return true;
}

bool recvFile(SOCKET fd, const char* fileName)
{
	size_t fileSize = 0;
	if (0 >= recv(fd, &fileSize, sizeof(size_t), 0))
	{
		err("recv");
		return false;
	}

	char* recvbuf = calloc(TRANSFER_SIZE, sizeof(char));
	if (!recvbuf)
	{
		printf("recvbuf memory failed\n");
		return false;
	}
	printf("接收文件中...\n");
	FILE* fp = fopen(fileName, "wb");
	for (size_t recvSize = 0; recvSize < fileSize;)
	{
		int ret = recv(fd, recvbuf, TRANSFER_SIZE, 0);
		fwrite(recvbuf, sizeof(char), ret, fp);
		recvSize += ret;
	}
	free(recvbuf);
	printf("fileSize:%lu\n", fileSize);
	fclose(fp);
	return true;
}

