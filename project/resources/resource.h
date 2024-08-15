#pragma once
#include "filesystem/file_helpers.h"
#include "core/globals.h"
#include <string>
class Resource
{
private:
	string name;
	string resource_path;
	string resource_source_path;
public:
	string get_name() { return name; }
	void set_name(string newName) { name = newName; }
	string get_path() { return resource_path; }
};

