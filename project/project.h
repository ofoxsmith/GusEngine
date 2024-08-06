#pragma once
#include <string>

class Project {
private:
	std::string absolutePath;
	std::string projectFile;
public:
	bool FindProjectFileInDir(std::string specificPath);
	bool InitProject();

};