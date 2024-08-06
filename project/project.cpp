#include "project.h"
#include "core/globals.h"
#include "filesystem/file_helpers.h"
using namespace std;
bool Project::FindProjectFileInDir(string specificPath)
{
	string path = "";
	if (specificPath != "" && file_helpers::file_exists(specificPath)) {
		path = specificPath;
		return true;
	}
	if (file_helpers::file_exists(".gproj")) {
		path = specificPath;
		return true;
	}
	std::vector<fs::path> files = file_helpers::get_files_in_dir();
	for (const auto& entry : files) {
		if (entry.has_extension() && entry.extension() == "gproj") {
			path = entry.string();
		}
	}
	
	if (path == "") return false;
	return true;
}

bool Project::InitProject()
{
	return false;
}
