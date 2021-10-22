#include "memory.hpp"

MEMORY_BLOCK* create_memory_block(const MEMORY_BASIC_INFORMATION& mbi) {
	MEMORY_BLOCK* block = new MEMORY_BLOCK;

	block->base_address = reinterpret_cast<unsigned char*>(mbi.BaseAddress);
	block->block_data = new unsigned char[mbi.RegionSize];
	block->block_sz = mbi.RegionSize;

	return block;
}

void delete_memory_block(const MEMORY_BLOCK* memory_block) {
	if (!memory_block) return;

	if (memory_block->block_data) {
		delete[] memory_block->block_data;
	}

	delete memory_block;
}

std::vector<unsigned char>dump_available_memory(const uint32_t pid) {
	std::vector<MEMORY_BLOCK*>block_list;
	std::vector<unsigned char>raw_data;

	MEMORY_BASIC_INFORMATION mbi;
	unsigned char* mem_address = 0x0;

	HANDLE process = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);

	if (process) {
		while (true) {
			if (VirtualQueryEx(process, mem_address, &mbi, sizeof(mbi)) == 0) break;

			if ((mbi.State & MEM_COMMIT) && (mbi.Protect & WRITEABLE_FLAGS)) {
				MEMORY_BLOCK* memory_block = create_memory_block(mbi);

				ReadProcessMemory(process, memory_block->base_address, memory_block->block_data, memory_block->block_sz, NULL);

				block_list.push_back(memory_block);
			}

			mem_address = static_cast<unsigned char*>(mbi.BaseAddress) + mbi.RegionSize;
		}

		for (auto& block : block_list) {
			for (int i = 0; i < block->block_sz; ++i) {
				raw_data.push_back(block->block_data[i]);
			}

			delete_memory_block(block);
		}

		CloseHandle(process);
	}

	return raw_data;
}