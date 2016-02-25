/*	Copyright © 2015 Lukyanau Maksim

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
#include "MemoryManager.h"
#include "Launcher.h"

using namespace cross;

#ifdef WIN

#undef new

void* operator new(size_t size){
	return MemoryManager::Instance()->Alloc(size, __FILE__, __LINE__);
}

void* operator new(size_t size, char* filename, unsigned long line){
	return MemoryManager::Instance()->Alloc(size, filename, line);
}

void* operator new[](size_t size){
	return MemoryManager::Instance()->Alloc(size, __FILE__, __LINE__);
}

void* operator new[](size_t size, char* filename, unsigned long line){
	return MemoryManager::Instance()->Alloc(size, filename, line);
}

void operator delete(void* p){
	MemoryManager::Instance()->Free(p);
}

void operator delete(void* p, char* filename, unsigned long line){
	MemoryManager::Instance()->Free(p);
}

void operator delete[](void* p){
	MemoryManager::Instance()->Free(p);
}

void operator delete[](void* p, char* filename, unsigned long line){
	MemoryManager::Instance()->Free(p);
}

#endif

const unsigned long		MemoryManager::check_code	= 0x12345678;
bool					MemoryManager::dead			= true;
MemoryManager			MemoryManager::instance;

MemoryManager* MemoryManager::Instance(){
	return &instance;
}

MemoryManager::MemoryManager():
	object_count(0)
{
	dead = false;
}

MemoryManager::~MemoryManager(){
	dead = true;
}

void* MemoryManager::Alloc(unsigned int size, char* filename, unsigned int line){
	if(!dead){
		SanityCheck();
		static int maxAlloc = MAX_ALLOC;
		if(object_count == maxAlloc - 1){
			maxAlloc = 0;
			throw CrossException("Maximum object allocated. Needs to redesign MemoryManager.");
		}

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

void MemoryManager::Free(void* address){
	if(!dead){
		SanityCheck();
		if(address == NULL){
			throw CrossException("Null pointer deletion");
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
		throw CrossException("Attempt to delete bad pointer");
	}else{
		free(address);
	}
}

unsigned long MemoryManager::Dump(){
	SanityCheck();
	unsigned long totalBytes = 0;
	for(unsigned int i = 0; i < object_count; i++){
		launcher->LogIt("%4d. 0x%08X: %d bytes(%s: %d)",
			i,
			(unsigned long)alloc_objects[i].address,
			alloc_objects[i].size,
			alloc_objects[i].filename, 
			alloc_objects[i].line);
		totalBytes += alloc_objects[i].size;
	}
	return totalBytes;
}

void MemoryManager::SanityCheck(){
	int count = 0;
	for(unsigned int i = 0; i < object_count; ++i){
		char* temp = (char*)alloc_objects[i].address;
		temp += alloc_objects[i].size;
		if(memcmp(temp, &check_code, 4) != 0){
			launcher->LogIt("Memory corrupted at 0x%08X: %d bytes(%s: %d)\n",
				(unsigned long)alloc_objects[i].address,
				alloc_objects[i].size,
				alloc_objects[i].filename,
				alloc_objects[i].line);
			count++;
		}
	}
	if(count > 0){
		launcher->LogIt("Total: %d corrupted buffers\n", count);
	}
}