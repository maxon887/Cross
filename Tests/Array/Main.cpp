#include "Cross.h"
#include "Internals/MemoryManager.h"
#include "Array.h"

#include <stdio.h>

using namespace cross;

void main() {
	
	MemoryManager::Instance()->Dump();
	getchar();
}