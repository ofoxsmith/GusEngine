#include "resource_loader.h"
#include "file_helpers.h"
#include "filesystem/res_fileparser.h"
#include "project/resources/propertyResource.h"

void ResourceLoader::Init() {
	dataCache.Init();
}

void ResourceLoader::Cleanup() {
	dataCache.Cleanup();
}

EngineDataCache ResourceLoader::dataCache{};
std::unordered_map<string, Resource*> ResourceLoader::loadedResources;
Resource* ResourceLoader::_loadPropertyResource(propertyResourceLoadMode mode, const string path) {
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
		resources::ShaderResourceOptions shaderOpts;
		shaderOpts.resourceName = resourceData.resourceName;
		shaderOpts.resourceSavedPath = resourceData.resourcePath;
		shaderOpts.sourceFilePath = resourceData.dataPath;
		shaderOpts.shaderLanguage = shaderOpts.LanguageGLSL;
		if (sourceType == "vert") shaderOpts.shaderStage = shaderOpts.StageVert;
		if (sourceType == "frag") shaderOpts.shaderStage = shaderOpts.StageFrag;
		if (sourceType == "tesc") shaderOpts.shaderStage = shaderOpts.StageTessControl;
		if (sourceType == "tese") shaderOpts.shaderStage = shaderOpts.StageTessEval;
		if (sourceType == "geom") shaderOpts.shaderStage = shaderOpts.StageGeom;
		if (sourceType == "comp") shaderOpts.shaderStage = shaderOpts.StageComp;
		shaderOpts.spirvBinary = Shader::CompileGLSLtoSPIRV(file_helpers::read_file_text(shaderOpts.sourceFilePath), shaderOpts.shaderLanguage, shaderOpts.shaderStage);
		Shader* s = new Shader(shaderOpts);
		return s;
	}

	Log.Error("ResourceLoader", "Invalid resource filepath: " + resourceData.dataPath);
	return nullptr;
}

// Loads a resource from the filesystem
Resource* ResourceLoader::_load(const string filePath) {
	if (loadedResources.contains(filePath)) {
		return loadedResources[filePath];
	}

	if (loadedResources.contains(filePath + ".pres")) {
		return loadedResources[filePath + ".pres"];
	}

	if (!file_helpers::file_exists(filePath)) {
		Log.Error("ResourceLoader", filePath + " does not exist.");
		return nullptr;
	}

	string resourceType = file_helpers::get_file_type(filePath);
	Resource* res;

	if (resourceType == "res") {
		// The file is a standard resource, and can be converted directly to a resource
		//loadedResources[filePath] = res;
	}

	if (resourceType == "pres") {
		// The file is a property resource, and contains engine settings for an external data file.
		res = ResourceLoader::_loadPropertyResource(propResLoad, filePath);
		loadedResources[filePath] = res;
	}


	// The file is external data, and must be encapsulated in a property resource
	bool presFileExists = file_helpers::file_exists(filePath + ".pres");
	if (presFileExists) {
		res = ResourceLoader::_loadPropertyResource(propResLoad, filePath + ".pres");
		loadedResources[filePath + ".pres"] = res;
	} else {
		res = ResourceLoader::_loadPropertyResource(propResLoadSrcOnly, filePath);
		loadedResources[filePath] = res;
	}

	if (res == nullptr) {
		Log.Error("ResourceLoader", "Failed to load resource from filepath: " + filePath);
		return nullptr;
	}

	return res;
}

