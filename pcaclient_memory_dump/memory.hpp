#pragma once
#include <vector>
#include <Windows.h>

#define WRITEABLE_FLAGS (PAGE_READWRITE | PAGE_WRITECOPY | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY)

struct MEMORY_BLOCK {
	unsigned char* base_address;
	unsigned char* block_data;
	size_t block_sz;
};

MEMORY_BLOCK* create_memory_block(const MEMORY_BASIC_INFORMATION& mbi);
void delete_memory_block(const MEMORY_BLOCK* memory_block);
std::vector<unsigned char>dump_available_memory(const uint32_t pid);