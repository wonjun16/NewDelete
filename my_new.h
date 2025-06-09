#pragma once

struct AllocInfo
{
	void* ptr;
	int size;
	char filename[128];
	int  line;
	bool array;
};

class MyNew
{
public:
	MyNew();
	~MyNew();

	void PushInfo(void* p, size_t size, const char* File, int Line, bool isArray);
	void DeleteInfo(const int index);

	AllocInfo AInfo[100];

	char ErrorLogs[100][256];
	int LogIndex;
private:
	int FindEmptyIndex() const noexcept;
};

void operator delete(void* p, size_t size);
void operator delete[](void* p);

void* operator new(size_t size, const char* File, int Line);
void* operator new[](size_t size, const char* File, int Line);
void operator delete(void* p, const char* File, int Line);
void operator delete[](void* p, const char* File, int Line);

#define new new(__FILE__, __LINE__)
