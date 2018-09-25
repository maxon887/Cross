#include "Cross.h"
#include "Internals/MemoryManager.h"

#include "Array.h"

#include <stdio.h>
#include <vector>

using namespace cross;

//template<class T>
//using ArrayTest = ArraySTD<T>;

template<class T>
using ArrayTest = cross::Array<T>;

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
		printf("Hard Operator =\n");
		memcpy(other.data, data, data_size);
	}

private:
	const int data_size = 10;
	char* data = nullptr;
};

void PrimitiveTest() {
	ArrayTest<int> array;
	printf("Initial Capacity - %d\n", array.Capacity());
	array.Add(1);
	printf("First Capacity - %d\n", array.Capacity());
	array.Add(2);
	printf("Second Capacity - %d\n", array.Capacity());
	array.Add(3);
	printf("Third Capacity - %d\n", array.Capacity());
}

void ObjectTest() {
	Hard hardOne;

	ArrayTest<Hard> array;
	printf("Initial Capacity(%d) and Size(%d)\n", array.Capacity(), array.Size());
	array.Add(hardOne);
	printf("First Capacity(%d) and Size(%d)\n", array.Capacity(), array.Size());
	array.Add(hardOne);
	printf("Second Capacity(%d) and Size(%d)\n", array.Capacity(), array.Size());
	array.Add(hardOne);
	printf("Third Capacity(%d) and Size(%d)\n", array.Capacity(), array.Size());
}

void main() {
	//PrimitiveTest();
	ObjectTest();
	MemoryManager::Instance()->Dump();
	getchar();
}