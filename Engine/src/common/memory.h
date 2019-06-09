#pragma once

#include <cstdlib>

void PrevMemoryInit();
void * PrevMemoryMalloc(std::size_t size, const char * file, std::size_t line);
void PrevMemoryFree(void * ptr, const char * file, std::size_t line);

#define malloc(s) PrevMemoryMalloc(s, __FILE__, __LINE__)
#define free(s) PrevMemoryFree(s, __FILE__, __LINE__)