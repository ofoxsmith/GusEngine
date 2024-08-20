#include "resource.h"

resources::Resource::Resource(string type, bool isProp, string path)
{
	_type = type;
	if (path != "") _saved = true;
	_resourcePath = path;
	_isSource = isProp;
}
