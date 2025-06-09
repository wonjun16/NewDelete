#include "my_new.h"
#include <Windows.h>
#include <ctime>
#include <stdio.h>

#undef new

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
	
	LogIndex = 0;

	for (int i = 0;i < 100;i++)
	{
		for (int j = 0;j < 256;j++)
		{
			ErrorLogs[i][j] = 0;
		}
	}
}

MyNew::~MyNew()
{
	for (int i = 0;i < 100;i++)
	{
		if (myNew.AInfo[i].ptr != 0)
		{
			snprintf(myNew.ErrorLogs[myNew.LogIndex], sizeof(myNew.ErrorLogs[myNew.LogIndex]),
				"LEAK %p %d %s : %d\n\0",
				myNew.AInfo[i].ptr,
				myNew.AInfo[i].size,
				myNew.AInfo[i].filename,
				myNew.AInfo[i].line);

			myNew.LogIndex++;
		}
	}

	time_t CurTime;
	tm Tm;
	time(&CurTime);
	localtime_s(&Tm, &CurTime);

	char FileName[256] = { 0, };
	snprintf(FileName, sizeof(FileName), "Alloc_%04d%02d%02d_%02d%02d%02d.txt",
		1900 + Tm.tm_year, 1 + Tm.tm_mon, Tm.tm_mday,
		Tm.tm_hour, Tm.tm_min, Tm.tm_sec);

	FILE* File;
	fopen_s(&File, FileName, "w");

	for (int i = 0;i < myNew.LogIndex;i++)
	{
		int size = 0;
		while (myNew.ErrorLogs[i][size] != 0)size++;
		fwrite(myNew.ErrorLogs[i], 1, size, File);
	}
	fclose(File);
}

void MyNew::PushInfo(void* p, size_t size, const char* File, int Line, bool isArray)
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

void MyNew::DeleteInfo(const int index)
{
	AInfo[index].array = 0;
	memset(AInfo[index].filename, 0, sizeof(AInfo[index].filename));
	AInfo[index].line = 0;
	AInfo[index].ptr = 0;
	AInfo[index].size = 0;
}

int MyNew::FindEmptyIndex() const noexcept
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

void operator delete(void* p, size_t size)
{
	bool isAlloc = 0;
	bool isArrayCor = 0;
	int index = -1;

	for (int i = 0;i < 100;i++)
	{
		if (myNew.AInfo[i].ptr == p || 
			(long long)myNew.AInfo[i].ptr == (long long)p - sizeof(p))
		{
			index = i;
			isAlloc = 1;
			if (myNew.AInfo[i].array == 0) isArrayCor = 1;
			break;
		}
	}

	if (isAlloc == 1 && isArrayCor == 1)
	{
		myNew.DeleteInfo(index);
		free(p);
	}
	else
	{
		if (isAlloc == 0)
		{
			snprintf(myNew.ErrorLogs[myNew.LogIndex], sizeof(myNew.ErrorLogs[myNew.LogIndex]),
				"NOALLOC %p\n\0",
				p);

			myNew.LogIndex++;
		}
		if (isAlloc == 1 && isArrayCor == 0)
		{
			snprintf(myNew.ErrorLogs[myNew.LogIndex], sizeof(myNew.ErrorLogs[myNew.LogIndex]),
				"ARRAY %p %d %s : %d\n\0",
				myNew.AInfo[index].ptr,
				myNew.AInfo[index].size,
				myNew.AInfo[index].filename,
				myNew.AInfo[index].line);

			myNew.LogIndex++;
		}
	}
}

void operator delete[](void* p)
{
	bool isAlloc = 0;
	bool isArrayCor = 0;
	int index = -1;

	for (int i = 0;i < 100;i++)
	{
		if (myNew.AInfo[i].ptr == p ||
			(long long)myNew.AInfo[i].ptr == (long long)p - sizeof(p))
		{
			index = i;
			isAlloc = 1;
			if (myNew.AInfo[i].array == 1) isArrayCor = 1;
			break;
		}
	}

	if (isAlloc == 1 && isArrayCor == 1)
	{
		myNew.DeleteInfo(index);
		free(p);
	}
	else
	{
		if (isAlloc == 0)
		{
			snprintf(myNew.ErrorLogs[myNew.LogIndex], sizeof(myNew.ErrorLogs[myNew.LogIndex]),
				"NOALLOC %p\n\0",
				p);

			myNew.LogIndex++;
		}
		if (isAlloc == 1 && isArrayCor == 0)
		{
			snprintf(myNew.ErrorLogs[myNew.LogIndex], sizeof(myNew.ErrorLogs[myNew.LogIndex]),
				"ARRAY %p %d %s : %d\n\0",
				myNew.AInfo[index].ptr,
				myNew.AInfo[index].size,
				myNew.AInfo[index].filename,
				myNew.AInfo[index].line);

			myNew.LogIndex++;
		}
	}
}

void* operator new(size_t size, const char* File, int Line)
{
	void* ptr = malloc(size);
	myNew.PushInfo(ptr, size, File, Line, 0);
	return ptr;
}

void* operator new[](size_t size, const char* File, int Line)
{
	void* ptr = malloc(size);
	myNew.PushInfo(ptr, size, File, Line, 1);
	return ptr;
}

void operator delete(void* p, const char* File, int Line)
{
}

void operator delete[](void* p, const char* File, int Line)
{
}
