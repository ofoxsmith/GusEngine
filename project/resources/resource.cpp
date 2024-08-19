#include "resource.h"

resources::Resource::Resource(string type, string path, string sourcePath)
{
	_type = type;
	if (path != "") _saved = true;
	_resourcePath = path;
	if (sourcePath != "") _hasSource = true;
	_resourceSourcePath = sourcePath;
}
