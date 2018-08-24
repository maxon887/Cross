#include "Cross.h"
#include "Internals/MemoryManager.h"
#include "Array.h"

#include <stdio.h>
#include <vector>

using namespace cross;

class Hard {
public:
	Hard() {
		printf("Hard Constructor\n");
		data = new char[data_size];
	}
	Hard(const Hard& other) {
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
	Hard hardTwo;
	hardTwo = hardOne;

	vector<Hard> vec(10);
	vec[0] = hardTwo;
}

void main() {
	Test();
	MemoryManager::Instance()->Dump();
	getchar();
}