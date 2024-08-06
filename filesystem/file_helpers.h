#pragma once
#include <vector>
#include <string>

class file_helpers {
public:
	static std::vector<char> read_file(const std::string& filename);
	static bool file_exists(const std::string& filename);
};