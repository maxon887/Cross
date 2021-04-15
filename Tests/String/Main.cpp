#include "Cross.h"
#include "Internals/MemoryManager.h"

#include <stdio.h>

using namespace cross;

void main() {
	String test = "This is a test string";

	MemoryManager::Instance()->Dump();
	getchar();
}