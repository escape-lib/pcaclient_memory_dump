#include <iostream>
#include "memory.hpp"
#include "util.hpp"

#define PCA_BLOCK_SZ 1024

int main(int argc, char* argv[])
{
	SetConsoleTitleA("pcaclient_memory_dump");
	std::cout << "PCACLIENT virtualmemory dump -> made by escape <3\n\n";
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	uint32_t explorer_pid = get_process_id_by_name(L"explorer.exe");
	if (explorer_pid == 0) {
		return hang_on_error("Could not locate explorer process.\n");
	}

	std::cout << "Explorer PID: (*) " << explorer_pid << '\n';

	std::vector<unsigned char>data = dump_available_memory(explorer_pid);
	if (data.size() == 0) {
		return hang_on_error("Critical virtualmemory read failure.\n");
	}

	std::cout << "RAW dumped data size: " << data.size() * sizeof(unsigned char) << " bytes\n";

	std::string data_s = "";
	for (auto& it : data) {
		data_s += it;
	}

	size_t begin_pos = data_s.find("TRACE,0000");
	if (begin_pos == std::string::npos) {
		return hang_on_error("Could not find PCACLIENT virtualmemory.\n");
	}

	std::vector<std::string>pca_entries = clean_pca_data(data_s.substr(begin_pos, PCA_BLOCK_SZ));

	std::cout << "PCACLIENT entry count: " << pca_entries.size() << "\n\n";
	std::cout << "Cleaned up virtualmemory:\n";

	SetConsoleTextAttribute(hConsole, 10);
	for (auto& it : pca_entries) {
		std::cout << it << '\n';
	}

	_getch();
	return 0;
}