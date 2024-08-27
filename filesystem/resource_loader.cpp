#include "resource_loader.h"
#include "file_helpers.h"
#include "filesystem/res_fileparser.h"
#include "project/resources/propertyResource.h"
#include "filesystem/engine_data_cache.h"
Resource* ResourceLoader::_loadPropertyResource(propertyResourceLoadMode mode, const string path) {
	// Determine the type of the data file
	resources::PropertyResource::ParsedPropertyResourceFile resourceData;
	if (mode == propResLoad) {
		resourceData = ResourceFileParser::LoadPropResource(path);
	}
	else if (mode == propResLoadSrcOnly) {
		resourceData.dataPath = path;
	}
	string sourceType = file_helpers::get_file_type(resourceData.dataPath);
	if (sourceType == "jpg" || sourceType == "jpeg" || sourceType == "png" || sourceType == "bmp" || sourceType == "psd" || sourceType == "tga"
		|| sourceType == "gif" || sourceType == "hdr" || sourceType == "pic" || sourceType == "ppm" || sourceType == "pgm") {


	}

	if (sourceType == "vert" || sourceType == "frag" || sourceType == "tesc" || sourceType == "tese" || sourceType == "geom" || sourceType == "comp" || sourceType == "spv") {
		Shader* s = new Shader(resourceData);
		s->Init();
		return s;
	}
	Log.Error("ResourceLoader", "Invalid resource filepath: " + resourceData.dataPath);
	return nullptr;
}

Resource* ResourceLoader::_load(const string filePath) {
	if (!file_helpers::file_exists(filePath)) {
		Log.Error("ResourceLoader", filePath + " does not exist.");
		return nullptr;
	}
	string resourceType = file_helpers::get_file_type(filePath);
	if (resourceType == "res") {
		// The file is a standard resource, and can be converted directly to a resource
	}

	if (resourceType == "pres") {
		// The file is a property resource, and contains engine settings for an external data file.
		return ResourceLoader::_loadPropertyResource(propResLoad, filePath);
	}

	// The file is external data, and must be encapsulated in a property resource
	bool fileExists = file_helpers::file_exists(filePath + ".pres");
	if (fileExists) {
		return ResourceLoader::_loadPropertyResource(propResLoad, filePath + ".pres");
	}
	else {
		return ResourceLoader::_loadPropertyResource(propResLoadSrcOnly, filePath);
	}
}

