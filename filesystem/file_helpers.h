#pragma once
#include <vector>
#include <string>
#include <filesystem>
namespace fs = std::filesystem;

using namespace std;
class file_helpers {
public:
	static vector<char> read_file(const string& filename);
	static bool file_exists(const string& filename);
	static vector<fs::path> get_files_in_dir(const string& dir = ".");
};