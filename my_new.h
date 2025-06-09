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

	void PushInfo(void* p, size_t size, char* File, int Line, bool isArray);
	void DeleteInfo();
private:
	AllocInfo AInfo[100];

	int FindEmptyIndex() const;
};

void* operator new(size_t size);
void* operator new[](size_t size);
void operator delete(void* p);
void operator delete[](void* p);

void* operator new(size_t size, char* File, int Line);
void* operator new[](size_t size, char* File, int Line);
void operator delete(void* p, char* File, int Line);
void operator delete[](void* p, char* File, int Line);

#define new(size) new(size, __FILE__, __LINE__) 