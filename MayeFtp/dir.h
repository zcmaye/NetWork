#pragma once

typedef struct Dir
{
	char rootDir[128];	//��Ŀ¼
	char curDir[128];	//��ǰ����Ŀ¼
}Dir;

Dir* createDir(const char* rootDir);