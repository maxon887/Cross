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
#include "MemoryManager.h"

#ifdef CROSS_MEMORY_PROFILE

#include <cstdarg>
#include <cstdlib>
#include <mutex>
#include <cstring>

using namespace cross;

#define START_MEMORY_OBJECTS_ARRAY_CAPACITY 100

#ifdef WIN
#   include "Windows.h"
#endif

std::mutex mut;

void* operator new(size_t size, const char* filename, U64 line) {
	return MemoryManager::Instance()->Alloc(size, filename, line);
}

void* operator new[](size_t size, const char* filename, U64 line) {
	return MemoryManager::Instance()->Alloc(size, filename, line);
}

void operator delete(void* p) noexcept {
	MemoryManager::Instance()->Free(p);
}

void operator delete[](void* p) noexcept {
	MemoryManager::Instance()->Free(p);
}

void operator delete(void* p, const char* filename, cross::U64 line) {
	MemoryManager::Instance()->Free(p);
}

void operator delete[](void* p, const char* filename, cross::U64 line) {
	MemoryManager::Instance()->Free(p);
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
	capacity = START_MEMORY_OBJECTS_ARRAY_CAPACITY;
	alloc_objects = (MemoryObject*)malloc(sizeof(MemoryObject) * (Size)capacity);
}

MemoryManager::~MemoryManager() {
	free(alloc_objects);
	dead = true;
}

void* MemoryManager::Alloc(U64 size, const char* filename, U64 line) {
	if(!dead) {
		mut.lock();
		if(object_count > capacity - 1) {
			capacity *= 2;
			alloc_objects = (MemoryObject*)realloc(alloc_objects, sizeof(MemoryObject) * (Size)capacity);
		}

		SanityCheck();

		alloc_objects[object_count].address = malloc((Size)(size + 4));
		alloc_objects[object_count].filename = filename;
		alloc_objects[object_count].line = line;
		alloc_objects[object_count].size = size;

		char* temp = (char*)alloc_objects[object_count].address;
		temp += size;
		memcpy(temp, &check_code, 4);

		mut.unlock();
		return alloc_objects[object_count++].address;
	} else {
		return malloc((Size)size);
	}
}

void* MemoryManager::ReAlloc(void* pointer, U64 size, const char* filename, U64 line) {
	if(!dead) {
		mut.lock();
		SanityCheck();

		MemoryObject* obj = FindObject(pointer);
		if(!obj)
		{
			Log("Can not find memory object");
			assert(false);//we also could rearrange new block of memory
		}
		obj->address = realloc(pointer, (Size)(size + 4));
		obj->filename = filename;
		obj->size = size;

		memcpy((char*)obj->address + size, &check_code, 4);
		mut.unlock();
		return obj->address;
	} else {
		return realloc(pointer, (Size)size);
	}
}

void MemoryManager::Free(void* address) {
	if(!dead) {
		mut.lock();
		SanityCheck();
		if(address == nullptr) {
			mut.unlock();
			return;
		}
		for(U64 i = 0; i < object_count; i++) {
			if(alloc_objects[i].address == address) {
				free(address);
				if(i != object_count - 1) {
					memcpy(alloc_objects + i, alloc_objects + object_count - 1, sizeof(MemoryObject));
				}
				object_count--;
				mut.unlock();
				return;
			}
		}
		free(address);
		mut.unlock();
	} else {
		free(address);
	}
}

void MemoryManager::Dump() {
	SanityCheck();
	U64 totalBytes = 0;
	for(U64 i = 0; i < object_count; i++) {
		Log("%4llu. 0x%08X: %llu bytes(%s: %llu)\n",
			i,
			(U64)alloc_objects[i].address,
			alloc_objects[i].size,
			alloc_objects[i].filename,
			alloc_objects[i].line);
		totalBytes += alloc_objects[i].size;
	}
	if(totalBytes != 0) {
		Log("Memory leak detected. Total - %llu bytes\n", totalBytes);
		assert(false);
	} else {
		Log("No memory leak detected\n");
	}
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
				(U64)alloc_objects[i].address,
				alloc_objects[i].size,
				alloc_objects[i].filename,
				alloc_objects[i].line);
			count++;
		}
	}
	if(count != 0) {
		Log("Sanity Check failed\nTotal: %d corrupted buffers\n", count);
		assert(false);
	}
}

MemoryManager::MemoryObject* MemoryManager::FindObject(void* address) {
	for(U64 i = 0; i < object_count; ++i) {
		if(alloc_objects[i].address == address) {
			return &alloc_objects[i];
		}
	}
	return nullptr;
}

void MemoryManager::Log(const char* msg, ...) {
	va_list params;
	constexpr int BufferSize = 4096;
	char buffer[BufferSize];
	va_start(params, msg);
	vsnprintf(buffer, BufferSize, msg, params);
#ifdef WIN
	OutputDebugStringA(buffer);
#else
	printf("%s", buffer);
#endif
	va_end(params);
}

void* StaticAlloc(cross::S64 size, const char* filename, U64 line) {
	void* result = MemoryManager::Instance()->Alloc(size, filename, line);
	return result;
}

void* StaticReAlloc(void* pointer, cross::S64 size, const char* filename, U64 line) {
	void* result = MemoryManager::Instance()->ReAlloc(pointer, size, filename, line);
	return result;
}
void StaticFree(void* pointer) {
	MemoryManager::Instance()->Free(pointer);
}


#else
#include <stdlib.h>

void* StaticAlloc(cross::S64 size, const char* filename, cross::U64 line) {
	return malloc((cross::Size)size);
}

void* StaticReAlloc(void* pointer, cross::S64 size, const char* filename, cross::U64 line) {
	return realloc(pointer, (cross::Size)size);
}
void StaticFree(void* pointer) {
	free(pointer);
}


#endif
