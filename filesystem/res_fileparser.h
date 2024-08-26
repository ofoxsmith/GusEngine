#pragma once
#include "core/globals.h"
#include "file_helpers.h"
#include <string>
#include "project/resources//propertyResource.h"
class ResourceFileParser {
public:
	static void LoadResource(const string filePath);
	static resources::PropertyResource::ParsedPropertyResourceFile LoadPropResource(const string filePath);
	void LoadSceneResource() { throw new runtime_error("not implemented"); }
};