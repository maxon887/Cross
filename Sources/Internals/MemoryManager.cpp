/*	Copyright © 2018 Maksim Lukyanov

	This file is part of Cross++ Game Engine.

	Cross++ Game Engine is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Cross++ is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with Cross++.  If not, see <http://www.gnu.org/licenses/>			*/
#include "Cross.h"
#ifdef CROSS_MEMORY_PROFILE

#include "MemoryManager.h"
#include "System.h"

#include <stdarg.h>
#include <stdlib.h>
#include <mutex>
#include <cstring>

using namespace cross;

#define START_MEMORY_OBJECTS_ARRAY_CAPACITY 100

#ifdef WIN
#   include "Windows.h"
#endif

#undef new

std::mutex mut;

void* operator new(size_t size) {
	mut.lock();
	void* result = MemoryManager::Instance()->Alloc(size, __FILE__, __LINE__);
	mut.unlock();
	return result;
}

void* operator new(size_t size, char* filename, U64 line) {
	mut.lock();
	void* result = MemoryManager::Instance()->Alloc(size, filename, line);
	mut.unlock();
	return result;
}

void* operator new[](size_t size) {
	mut.lock();
	void* result = MemoryManager::Instance()->Alloc(size, __FILE__, __LINE__);
	mut.unlock();
	return result;
}

void* operator new[](size_t size, char* filename, U64 line) {
	mut.lock();
	void* result = MemoryManager::Instance()->Alloc(size, filename, line);
	mut.unlock();
	return result;
}

void operator delete(void* p) noexcept {
	mut.lock();
	MemoryManager::Instance()->Free(p);
	mut.unlock();
}

void operator delete(void* p, char* filename, U64 line) {
	mut.lock();
	MemoryManager::Instance()->Free(p);
	mut.unlock();
}

void operator delete[](void* p) noexcept {
	mut.lock();
	MemoryManager::Instance()->Free(p);
	mut.unlock();
}

void operator delete[](void* p, char* filename, U64 line) {
	mut.lock();
	MemoryManager::Instance()->Free(p);
	mut.unlock();
}

const U64				MemoryManager::check_code	= 0x12345678;
bool					MemoryManager::dead			= true;
MemoryManager			MemoryManager::instance;

MemoryManager* MemoryManager::Instance() {
	return &instance;
}

MemoryManager::MemoryManager():
	object_count(0)
{
	dead = false;
	capacity = START_MEMORY_OBJECTS_ARRAY_CAPACITY;
	alloc_objects = (MemoryObject*)malloc((size_t)(sizeof(MemoryObject) * capacity));
}

MemoryManager::~MemoryManager() {
	dead = true;
}

void* MemoryManager::Alloc(U64 size, const char* filename, U64 line) {
	if(!dead) {

		if(object_count > capacity - 1) {
			capacity *= 2;
			alloc_objects = (MemoryObject*)realloc(alloc_objects, (size_t)(sizeof(MemoryObject) * capacity));
		}

		SanityCheck();

		alloc_objects[object_count].address = malloc((size_t)(size + 4));
		alloc_objects[object_count].filename = filename;
		alloc_objects[object_count].line = line;
		alloc_objects[object_count].size = size;

		char* temp = (char*)alloc_objects[object_count].address;
		temp += size;
		memcpy(temp, &check_code, 4);

		return alloc_objects[object_count++].address;
	} else {
		return malloc((size_t)size);
	}
}

void* MemoryManager::ReAlloc(void* pointer, U64 size, const char* filename, U64 line) {
	if(!dead) {
		SanityCheck();

		MemoryObject& obj = FindObject(pointer);
		obj.address = realloc(pointer, (size_t)(size + 4));
		obj.filename = filename;
		obj.size = size;

		memcpy((char*)obj.address + size, &check_code, 4);

		return obj.address;
	} else {
		return realloc(pointer, (size_t)size);
	}
}

void MemoryManager::Free(void* address) {
	if(!dead) {
		SanityCheck();
		if(address == nullptr) {
			return;
		}
		for(U64 i = 0; i < object_count; i++) {
			if(alloc_objects[i].address == address) {
				free(address);
				if(i != object_count - 1) {
					memcpy(alloc_objects + i, alloc_objects + object_count - 1, sizeof(MemoryObject));
				}
				object_count--;
				return;
			}
		}
		CROSS_ASSERT(false, "Attempt to delete bad pointer\n");
	} else {
		free(address);
	}
}

U64 MemoryManager::Dump() {
	SanityCheck();
	U64 totalBytes = 0;
	for(U64 i = 0; i < object_count; i++) {
		Log("%4llu. 0x%08X: %llu bytes(%s: %llu)\n",
			i,
			(unsigned long)alloc_objects[i].address,
			alloc_objects[i].size,
			alloc_objects[i].filename,
			alloc_objects[i].line);
		totalBytes += alloc_objects[i].size;
	}
	if(totalBytes != 0) {
		Log("Memory leak detected(%llu bytes)\n", totalBytes);
		assert(false);
	}
	return totalBytes;
}

U64 MemoryManager::AllocatedMemory() const {
	U64 size = 0;
	for(U64 i = 0; i < object_count; i++) {
		size += alloc_objects[i].size;
	}
	return size;
}

void MemoryManager::SanityCheck() {
	U64 count = 0;
	for(U64 i = 0; i < object_count; ++i) {
		char* temp = (char*)alloc_objects[i].address;
		temp += alloc_objects[i].size;
		if(memcmp(temp, &check_code, 4) != 0) {
			Log("Memory corrupted at 0x%08X: %d bytes(%s: %d)\n",
				(unsigned long)alloc_objects[i].address,
				alloc_objects[i].size,
				alloc_objects[i].filename,
				alloc_objects[i].line);
			count++;
			Log("Sanity Check failed");
			assert(false);
		}
	}
	CROSS_ASSERT(count == 0,"Sanity Check failed\nTotal: # corrupted buffers\n", count);
}

MemoryObject& MemoryManager::FindObject(void* address) {
	for(U64 i = 0; i < object_count; ++i) {
		if(alloc_objects[i].address == address) {
			return alloc_objects[i];
		}
	}
	static MemoryObject bad_object;
	CROSS_RETURN(false, bad_object, "Can not find memory object");
}

void MemoryManager::Log(const char* msg, ...) {
	va_list params;
	char buffer[4096];
	va_start(params, msg);
	vsprintf(buffer, msg, params);
#ifdef WIN
	OutputDebugStringA(buffer);
#endif
	va_end(params);
}

#else
#include <stdlib.h>

void* StaticAlloc(cross::U64 size) {
	return malloc((size_t)size);
}

void* StaticReAlloc(void* pointer, cross::U64 size) {
	return realloc(pointer, (size_t)size);
}
void StaticFree(void* pointer) {
	free(pointer);
}


#endif
