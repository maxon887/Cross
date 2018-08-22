#include "Cross.h"
#include "Internals/MemoryManager.h"

#include <stdio.h>

using namespace cross;

void main() {
	
	MemoryManager::Instance()->Dump();
	getchar();
}