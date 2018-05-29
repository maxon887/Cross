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
#ifdef CROSS_MEMORY_PROFILE

#define CROSS_ALLOC(size) MemoryManager::Instance()->Alloc(size, __FILE__, __LINE__)
#define CROSS_REALLOC(pointer, size) MemoryManager::Instance()->ReAlloc(pointer, size, __FILE__, __LINE__)
#define CROSS_FREE(pointer) MemoryManager::Instance()->Free(pointer)

void* operator new(size_t size);
void* operator new(size_t size, char* filename, cross::U64 line);
void* operator new[](size_t size);
void* operator new[](size_t size, char* filename, cross::U64 line);
void operator delete(void* p) noexcept;
void operator delete(void* p, char* filename, cross::U64 line);
void operator delete[](void* p) noexcept;
void operator delete[](void* p, char* filename, cross::U64 line);

#define new new(__FILE__, __LINE__)

namespace cross{

struct MemoryObject {
	void* address;
	U64 size;
	const char* filename;
	U64 line;
};

class MemoryManager {
public:
	static MemoryManager* Instance();
	void* Alloc(U64 size, const char* filename, U64 line);
	void* ReAlloc(void* pointer, U64 size, const char* filename, U64 line);
	void Free(void* address);
	U64 Dump();

	U64 AllocatedMemory() const;

private:
	static const U64 check_code;
	static bool dead;
	static MemoryManager instance;

	U64 object_count;
	U64 capacity;
	MemoryObject* alloc_objects;

	MemoryManager();
	~MemoryManager();

	void SanityCheck();
	MemoryObject& FindObject(void* address);
	void Log(const char* msg, ...);
};

}

#else

void* StaticAlloc(cross::U64 size);
void* StaticReAlloc(void* pointer, cross::U64 size);
void StaticFree(void* pointer);

#define CROSS_ALLOC(size) StaticAlloc(size)
#define CROSS_REALLOC(pointer, size) StaticReAlloc(pointer, size)
#define CROSS_FREE(pointer) StaticFree(pointer)

#endif
