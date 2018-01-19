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
#pragma once
#if defined(CROSS_DEBUG) && !defined(EDITOR)
#ifdef WIN

void* operator new(size_t size);
void* operator new(size_t size, char* filename, unsigned long line);
void* operator new[](size_t size);
void* operator new[](size_t size, char* filename, unsigned long line);
void operator delete(void* p) noexcept;
void operator delete(void* p, char* filename, unsigned long line);
void operator delete[](void* p) noexcept;
void operator delete[](void* p, char* filename, unsigned long line);

#define new new(__FILE__, __LINE__)

#endif

namespace cross{

struct MemoryObject{
	void* address;
	unsigned long size;
	const char* filename;
	unsigned long line;
};

class MemoryManager{
public:
	static MemoryManager* Instance();
	void* Alloc(unsigned long size, const char* filename, unsigned long line);
	void Free(void* address);
	unsigned long Dump();
	
private:
	static const unsigned long check_code;
	static bool dead;
	static MemoryManager instance;

	unsigned int object_count;
	unsigned int capacity;
	MemoryObject* alloc_objects;

	MemoryManager();
	~MemoryManager();

	void SanityCheck();
	void Log(const char* msg, ...);
};

}

#endif // CROSS_DEBUG
