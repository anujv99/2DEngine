#include "pch.h"
#include "memory.h"

#include <new>
#include <memory.h>
#include <stddef.h>
#include <new.h>

#include <crtdbg.h>
#include <signal.h>

void PrevAbort(int signal) {
	DebugBreak();
	abort();
}

void PrevMemoryInit() {
	_set_new_mode(0);
	signal(SIGABRT, PrevAbort);
	_set_abort_behavior(0, _WRITE_ABORT_MSG);
}

#pragma warning(push)
#pragma warning(disable:4290)

void * operator new(std::size_t size) noexcept(false) { return PrevMemoryMalloc(size, "", 0); }
void * operator new(std::size_t size, const std::nothrow_t & nothrowValue) throw() { return PrevMemoryMalloc(size, "", 0); }

void operator delete(void * ptr) throw() { return PrevMemoryFree(ptr, "", 0); }
void operator delete(void * ptr, const std::nothrow_t & nothrow_constant) throw() { return PrevMemoryFree(ptr, "", 0); }

void * operator new[](std::size_t size) noexcept(false) { return PrevMemoryMalloc(size, "", 0); }
void * operator new[](std::size_t size, const std::nothrow_t & nothrow_value) throw() { return PrevMemoryMalloc(size, "", 0); }

void operator delete[](void * ptr) throw() { return PrevMemoryFree(ptr, "", 0); }
void operator delete[](void * ptr, const std::nothrow_t & nothrow_constant) throw() { return PrevMemoryFree(ptr, "", 0); }

#pragma warning(pop)

void * PrevMemoryMalloc(std::size_t size, const char * file, std::size_t line) {
	return (malloc)(size);
}

void PrevMemoryFree(void * ptr, const char * file, std::size_t line) {
	(free)(ptr);
}