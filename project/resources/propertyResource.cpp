#include "propertyResource.h"

resources::PropertyResource::PropertyResource(string type, string sourcePath, string path): Resource("Shader", true, path) {
	_sourcePath = sourcePath;
}
