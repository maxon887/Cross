#include "Cross.h"
#include "Internals/MemoryManager.h"

#include "../Sources/Experimental/Array.h"

#include <stdio.h>
#include <vector>

using namespace cross;
//
//template<class T>
//using ArrayTest = ArraySTD<T>;

template<class T>
using ArrayTest = cross::Array<T>;

class Hard {
public:
	Hard() {
		//printf("Hard Constructor\n");
		data = new char[data_size];
	}
	Hard(const Hard& other) {
		//printf("Hard Copy Constructor\n");
		data = new char[data_size];
		memcpy(data, other.data, data_size);
	}
	Hard(Hard&& other) {
		//printf("Hard Move Constructor\n");
		data = other.data;
		other.data = nullptr;
	}
	~Hard() {
		//printf("Hard Destructor\n");
		delete data;
	}

	void Set(const char* info) {
		strcpy(data, info);
	}

	char* Get() {
		return data;
	}

	void operator = (const Hard& other) {
		printf("Hard Operator =\n");
		memcpy(other.data, data, data_size);
	}

private:
	const int data_size = 25;
	char* data = nullptr;
};

void PrimitiveTest() {
	printf("========= Primitive Test ==========\n");

	ArrayTest<int> array;
	printf("Initial Capacity - %d\n", array.Capacity());
	array.Add(1);
	printf("First Capacity - %d\n", array.Capacity());
	array.Add(2);
	printf("Second Capacity - %d\n", array.Capacity());
	array.Add(3);
	printf("Third Capacity - %d\n", array.Capacity());

	printf("Third Element is - %d\n", array[2]);

	ArrayTest<float> defValues(5, 3.14f);
	defValues[3] = 6.26666f;

	for(float f : defValues) {
		printf("Interative float loop - %f\n", f);
	}

	Array<int> inits = { 1, 2, 3, -5 };
	printf("Second item is - (%d)", inits[1]);
	printf("Fouth item is - (%d)", inits[3]);
}

void ObjectTest() {
	printf("========= Object Test ==========\n");

	Hard hardOne;
	hardOne.Set("Some Info");

	ArrayTest<Hard> array;
	printf("Initial Capacity(%d) and Size(%d)\n", array.Capacity(), array.Size());
	array.Add(hardOne);
	printf("First Capacity(%d) and Size(%d)\n", array.Capacity(), array.Size());
	array.Add(hardOne);
	printf("Second Capacity(%d) and Size(%d)\n", array.Capacity(), array.Size());
	array.Add(hardOne);
	printf("Third Capacity(%d) and Size(%d)\n", array.Capacity(), array.Size());

	printf("First Object Contains (%s)\n", array[0].Get());

	array[1].Set("Second Info");
	array[2].Set("Another Useless Info");

	for(int i = 0; i < array.Size(); i++) {
		printf("%d element contains - %s\n", i, array[i].Get());
	}

	for(Hard& hard : array) {
		printf("Interative loop - %s\n", hard.Get());
	}

	ArrayTest<Hard> defValues(5, hardOne);
	defValues[3].Set("Not the same info");
	for(Hard copy : defValues) {
		printf("Interative def values loop - %s\n", copy.Get());
	}

	//ArrayTest<Hard>::Iterator iter;
	//iter = array.Begin();
	//while(iter != array.End()) {
	//	printf("Interative loop - %s\n", (*iter).Get());
	//	iter++;
	//}
}

void main() {
	PrimitiveTest();
	ObjectTest();
	MemoryManager::Instance()->Dump();
	getchar();
}