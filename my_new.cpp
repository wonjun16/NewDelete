#include "my_new.h"

#include <Windows.h>
#include <stdio.h>
MyNew myNew;

MyNew::MyNew()
{
	for (int i = 0; i < 100; i++)
	{
		AInfo[i].array = 0;
		memset(AInfo[i].filename, 0, sizeof(AInfo[i].filename));
		AInfo[i].line = 0;
		AInfo[i].ptr = 0;
		AInfo[i].size = 0;
	}
}

MyNew::~MyNew()
{
}

void MyNew::PushInfo(void* p, size_t size, char* File, int Line, bool isArray)
{
	int index = FindEmptyIndex();
	if (index != -1)
	{
		AInfo[index].array = isArray;
		strcpy_s(AInfo[index].filename, File);
		AInfo[index].line = Line;
		AInfo[index].ptr = p;
		AInfo[index].size = size;
	}
}

void MyNew::DeleteInfo()
{
}

int MyNew::FindEmptyIndex() const
{
	int index = -1;
	for (int i = 0; i < 100; i++)
	{
		if (AInfo[i].ptr == 0)
		{
			index = i;
			break;
		}
	}
	return index;
}

void* operator new(size_t size)
{
	return nullptr;
}

void* operator new(size_t size, char* File, int Line)
{
	void* ptr = malloc(size);
	myNew.PushInfo(ptr, size, File, Line, 0);
	return ptr;
}

void* operator new[](size_t size, char* File, int Line)
{
	printf("size : %d", size);
	//void* ptr = malloc(size);
	//myNew.PushInfo(ptr, size, File, Line, 1);
	return nullptr;
}

void operator delete(void* p, char* File, int Line)
{
}

void operator delete[](void* p, char* File, int Line)
{
}
