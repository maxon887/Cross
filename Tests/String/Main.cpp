#include <stdio.h>
#include "Cross.h"
#include "Internals/MemoryManager.h"

using namespace cross;

void main() {
	MemoryManager::Instance()->Dump();
	getchar();
}