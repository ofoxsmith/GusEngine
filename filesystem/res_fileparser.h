#pragma once
#include "core/globals.h"
#include "file_helpers.h"
#include <string>
#include "project/resources/propertyResource.h"
class resource_file_parser {
public:
	static void load_resource(const string filePath);
	static resources::PropertyResource::ParsedPropertyResourceFile load_prop_resource(const string filePath);
	void load_scene_resource() { throw new runtime_error("not implemented"); }
};