#include "resource_loader.h"


Shader* ResourceLoader::loadShaderFile(const string filePath)
{
	Shader* loaded = new Shader(filePath);
	return loaded;
}

Resource* ResourceLoader::Load(const string filePath) {
	if (!file_helpers::file_exists(filePath)) {
		Log.Error("ResourceLoader", filePath + " does not exist.");
		return nullptr;
	}

	string resourceType = file_helpers::get_file_type(filePath);
	if (resourceType == "res") {
		// The file is a standard resource, and can be converted directly to a resource
	}
	if (resourceType == "pres") {
		// The file is a property resource, and contains engine settings for an external data resource
	}

	// The file is an external data resource, and must be encapsulated in a property resource
	
	if (resourceType == "jpg" || resourceType == "jpeg" || resourceType == "png" || resourceType == "bmp" || resourceType == "psd" || resourceType == "tga"
		|| resourceType == "gif" || resourceType == "hdr" || resourceType == "pic" || resourceType == "ppm" || resourceType == "pgm") {
		

	}

	if (resourceType == "glsl" || resourceType == "vert" || resourceType == "frag" || resourceType == "spv") {
		return loadShaderFile(filePath);
	}

	Log.Error("ResourceLoader", filePath + " is not a valid resource type.");
	return nullptr;
}
