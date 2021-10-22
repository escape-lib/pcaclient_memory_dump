#pragma once

#include <Windows.h>
#include <TlHelp32.h>
#include <iostream>
#include <conio.h>
#include <vector>
#include <sstream>

std::vector<std::string>clean_pca_data(std::string data);
uint32_t get_process_id_by_name(std::wstring pname);
int hang_on_error(std::string error);