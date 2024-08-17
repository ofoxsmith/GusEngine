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
	static string get_file_name(string filepath);
	static string read_file_text(string filepath);
	static vector<char> read_file_binary(string filepath);
	static bool file_exists(string filepath);
	static string get_file_type(string filepath);
	static vector<fs::path> get_files_in_dir(string dir = ".");
};