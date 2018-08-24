#include "Cross.h"
#include "Internals/MemoryManager.h"

#include "Array.h"

#include <stdio.h>
#include <vector>

using namespace cross;

template<class T>
using ArrayTest = ArraySTD<T>;

class Hard {
public:
	Hard() {
		printf("Hard Constructor\n");
		data = new char[data_size];
	}
	Hard(const Hard& other) {
		printf("Hard Copy Constructor\n");
		data = new char[data_size];
		memcpy(other.data, data, data_size);
	}
	~Hard() {
		printf("Hard Destructor\n");
		delete data;
	}

	void operator = (const Hard& other) {
		memcpy(other.data, data, data_size);
	}

private:
	const int data_size = 10;
	char* data = nullptr;
};

void Test() {
	Hard hardOne;
	Hard hardTwo = hardOne;

	ArrayTest<Hard> array;
	array.Add(hardOne);
}

void main() {
	Test();
	MemoryManager::Instance()->Dump();
	getchar();
}