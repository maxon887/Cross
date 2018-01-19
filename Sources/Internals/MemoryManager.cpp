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

#include <stdarg.h>
#include <stdlib.h>
#include <mutex>

#if defined(CROSS_DEBUG) && !defined(EDITOR)

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

void* operator new(size_t size, char* filename, unsigned long line) {
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

void* operator new[](size_t size, char* filename, unsigned long line) {
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

void operator delete(void* p, char* filename, unsigned long line) {
	mut.lock();
	MemoryManager::Instance()->Free(p);
	mut.unlock();
}

void operator delete[](void* p) noexcept {
	mut.lock();
	MemoryManager::Instance()->Free(p);
	mut.unlock();
}

void operator delete[](void* p, char* filename, unsigned long line) {
	mut.lock();
	MemoryManager::Instance()->Free(p);
	mut.unlock();
}

const unsigned long		MemoryManager::check_code	= 0x12345678;
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
	alloc_objects = (MemoryObject*)malloc(sizeof(MemoryObject) * capacity);
}

MemoryManager::~MemoryManager(){
	dead = true;
}

void* MemoryManager::Alloc(unsigned long size, const char* filename, unsigned long line) {
	if(!dead){

		if(object_count > capacity - 1){
			capacity *= 2;
			alloc_objects = (MemoryObject*)realloc(alloc_objects, sizeof(MemoryObject) * capacity);
		}

		SanityCheck();

		alloc_objects[object_count].address = malloc(size + 4);
		alloc_objects[object_count].filename = filename;
		alloc_objects[object_count].line = line;
		alloc_objects[object_count].size = size;
	
		char* temp = (char*)alloc_objects[object_count].address;
		temp += size;
		memcpy(temp, &check_code, 4);
	
		return alloc_objects[object_count++].address;
	}else{
		return malloc(size);
	}
}

void MemoryManager::Free(void* address) {
	if(!dead){
		SanityCheck();
		if(address == NULL){
			return;
		}
		for(unsigned int i = 0; i < object_count; i++){
			if(alloc_objects[i].address == address){
				free(address);
				if(i != object_count - 1){
					memcpy(alloc_objects + i, alloc_objects + object_count - 1, sizeof(MemoryObject));
				}
				object_count--;
				return;
			}
		}
		CROSS_ASSERT(false, "Attempt to delete bad pointer\n");
	}else{
		free(address);
	}
}

unsigned long MemoryManager::Dump() {
	SanityCheck();
	unsigned long totalBytes = 0;
	for(unsigned int i = 0; i < object_count; i++) {
		Log("%4d. 0x%08X: %d bytes(%s: %d)\n",
			i,
			(unsigned long)alloc_objects[i].address,
			alloc_objects[i].size,
			alloc_objects[i].filename, 
			alloc_objects[i].line);
		totalBytes += alloc_objects[i].size;
	}
	Log("Memory lick detected(%u bytes)\n", totalBytes);
	CROSS_ASSERT(totalBytes == 0, "Memory lick detected(%ud bytes)", totalBytes);
	return totalBytes;
}

unsigned long MemoryManager::AllocatedMemory() const {
	unsigned long size = 0;
	for(unsigned int i = 0; i <object_count; i++) {
		size += alloc_objects[i].size;
	}
	return size;
}

void MemoryManager::SanityCheck(){
	int count = 0;
	for(unsigned int i = 0; i < object_count; ++i) {
		char* temp = (char*)alloc_objects[i].address;
		temp += alloc_objects[i].size;
		if(memcmp(temp, &check_code, 4) != 0) {
			Log("Memory corrupted at 0x%08X: %d bytes(%s: %d)\n",
				(unsigned long)alloc_objects[i].address,
				alloc_objects[i].size,
				alloc_objects[i].filename,
				alloc_objects[i].line);
			count++;
			CROSS_ASSERT(false, "Sanity Check failed");
		}
	}
	CROSS_ASSERT(count == 0,"Sanity Check failed\nTotal: %d corrupted buffers\n", count);
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

#endif // CROSS_DEBUG
