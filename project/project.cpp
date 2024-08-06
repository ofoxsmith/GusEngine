#include "project.h"
#include "../filesystem/file_helpers.h"

bool Project::FindProjectFileInDir(std::string specificPath = "")
{
	if (specificPath != "" && file_helpers::file_exists(specificPath)) {
		projectFile = specificPath;
		return true;
	}
	if (file_helpers::file_exists(".gproj")) {
		projectFile = specificPath;
		return true;
	}
	// TODO search for any .gproj file if not found
	return false;
}

bool Project::InitProject()
{
	return false;
}
