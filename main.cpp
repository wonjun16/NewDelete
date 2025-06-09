#include "my_new.h"
#include <stdio.h>

class A
{
public:
	int _a;

	A()
	{
		_a = 5;
	}
	~A() { printf("asd\n"); };
};

int main()
{
	A* a = new A[10];
	delete a;
	delete[] a;

	A* b = new A();

	delete b;

	A* c = new A();

	c = c + 14;

	delete c;
	return 0;
}