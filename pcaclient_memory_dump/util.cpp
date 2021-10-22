#include "util.hpp"

std::vector<std::string>clean_pca_data(std::string data) {
	std::vector<std::string>split_sections;

	std::stringstream stream(data);
	std::string section;
	for (char s : data) {
		while (std::getline(stream, section, ',')) {
			if (section.find(":\\") != std::string::npos) split_sections.push_back(section);
		}
	}

	return split_sections;
}

uint32_t get_process_id_by_name(std::wstring pname) {
	PROCESSENTRY32 entry;
	entry.dwSize = sizeof(PROCESSENTRY32);

	HANDLE list_snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

	Process32First(list_snapshot, &entry);
	do {
		if (wcscmp(entry.szExeFile, pname.c_str()) == 0) {
			CloseHandle(list_snapshot);

			return entry.th32ProcessID;
		}

	} while (Process32Next(list_snapshot, &entry));

	CloseHandle(list_snapshot);
	return 0;
}

int hang_on_error(std::string error) {
	std::cerr << error;
	_getch();
	return -1;
}