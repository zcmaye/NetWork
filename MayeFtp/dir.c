#include "dir.h"
#include<malloc.h>
#include<string.h>
Dir* createDir(const char* rootDir)
{
	Dir* dir = calloc(1,sizeof(Dir));
	if (dir == NULL)
		return NULL;
	strcpy_s(dir->rootDir, sizeof(dir->rootDir), rootDir);
	strcpy_s(dir->curDir, sizeof(dir->curDir), rootDir);
	return dir;
}
