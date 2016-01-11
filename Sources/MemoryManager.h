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
#pragma once

#define MAX_ALLOC 1000

void* operator new(size_t size);
void* operator new(size_t size, char* filename, unsigned long line);
void* operator new[](size_t size);
void* operator new[](size_t size, char* filename, unsigned long line);
void operator delete(void* p);
void operator delete[](void* p);

#define DEBUG_NEW new(__FILE__, __LINE__)
#define new DEBUG_NEW

namespace cross{

struct MemoryObject{
	void* address;
	unsigned int size;
	char* filename;
	unsigned int line;
};

class MemoryManager{
public:
	static MemoryManager* Instance();
	void* Alloc(unsigned int size, char* filename, unsigned int line);
	void Free(void* address);
	unsigned long Dump();
private:
	unsigned int object_count;
	MemoryObject alloc_objects[MAX_ALLOC];

	MemoryManager();

	static MemoryManager instance;
	void SanityCheck();
};

}