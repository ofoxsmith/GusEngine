#include "resource_loader.h"
#include "file_helpers.h"
#include "filesystem/res_fileparser.h"

Resource* ResourceLoader::_loadPropertyResource(propertyResourceLoadMode mode, const string path) {
	// Determine the type of the data file
	string sourcePath;
	string resourcePath;
	ResourceFileParser::ParsedPropertyResourceFile resourceData;
	if (mode == propResLoad) {
		resourcePath = path;
		sourcePath = path.substr(0, path.length() - 5);
		resourceData = ResourceFileParser::LoadPropResource(resourcePath);
	}
	else if (mode == propResLoadSrcOnly) {
		resourcePath = "";
		sourcePath = path;
	}


	string sourceType = file_helpers::get_file_type(sourcePath);
	if (sourceType == "jpg" || sourceType == "jpeg" || sourceType == "png" || sourceType == "bmp" || sourceType == "psd" || sourceType == "tga"
		|| sourceType == "gif" || sourceType == "hdr" || sourceType == "pic" || sourceType == "ppm" || sourceType == "pgm") {


	}

	if (sourceType == "vert" || sourceType == "frag" || sourceType == "tesc" || sourceType == "tese" || sourceType == "geom" || sourceType == "comp" || sourceType == "spv") {
		Shader* s = new Shader(resourcePath, sourcePath);
		s->Init();
		return s;
	}

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

