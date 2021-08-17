#pragma once

typedef struct Dir
{
	char rootDir[128];	//根目录
	char curDir[128];	//当前操作目录
}Dir;

Dir* createDir(const char* rootDir);