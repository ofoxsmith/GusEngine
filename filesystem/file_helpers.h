#pragma once
#include <vector>
#include <string>
#include <filesystem>
namespace fs = std::filesystem;

using namespace std;
class file_helpers {
	private:
	static inline void remove_path_prefix(string& path);
public:
	static vector<char> read_file(string filename);
	static bool file_exists(string filename);
	static string get_file_type(string filename);
	static vector<fs::path> get_files_in_dir(string dir = ".");
};