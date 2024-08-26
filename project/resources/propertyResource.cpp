#include "propertyResource.h"

resources::PropertyResource::PropertyResource(string type, ParsedPropertyResourceFile data): Resource(type, true, data.resourcePath) {
	_sourcePath = data.dataPath;
}
