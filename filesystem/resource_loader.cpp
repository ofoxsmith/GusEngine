#include "resource_loader.h"
#include "file_helpers.h"
#include "engine_io.h"
void ResourceLoader::Init() {
	//dataCache.Init();
}

void ResourceLoader::Cleanup() {
	//dataCache.Cleanup();
}

EngineDataCache ResourceLoader::dataCache{};
std::unordered_map<string, Resource*> ResourceLoader::loadedResources;
//Resource* ResourceLoader::_loadPropertyResource(propertyResourceLoadMode mode, const string path) {
//	resources::PropertyResource::ParsedPropertyResourceFile resourceData;
//
//	if (mode == propertyResourceLoadMode::propResLoad) {
//		//resourceData = resource_file_parser::load_prop_resource(path);
//	}
//	else if (mode == propertyResourceLoadMode::propResLoadSrcOnly) {
//		resourceData.dataPath = path;
//	}
//
//	string sourceType = file_helpers::get_file_type(resourceData.dataPath);
//
//	if (sourceType == "jpg" || sourceType == "jpeg" || sourceType == "png" || sourceType == "bmp" || sourceType == "psd" || sourceType == "tga"
//		|| sourceType == "gif" || sourceType == "hdr" || sourceType == "pic" || sourceType == "ppm" || sourceType == "pgm") {
//
//
//	}
//
//	if (sourceType == "vert" || sourceType == "frag" || sourceType == "tesc" || sourceType == "tese" || sourceType == "geom" || sourceType == "comp" || sourceType == "spv") {
//		resources::ShaderResourceOptions shaderOpts;
//		shaderOpts.resourceName = resourceData.resourceName;
//		shaderOpts.resourceSavedPath = resourceData.resourcePath;
//		shaderOpts.sourceFilePath = resourceData.dataPath;
//		shaderOpts.language = ShaderResourceOptions::ShaderLanguage::LanguageGLSL;
//		if (sourceType == "vert") shaderOpts.stage = ShaderResourceOptions::ShaderStage::StageVert;
//		if (sourceType == "frag") shaderOpts.stage = ShaderResourceOptions::ShaderStage::StageFrag;
//		if (sourceType == "tesc") shaderOpts.stage = ShaderResourceOptions::ShaderStage::StageTessControl;
//		if (sourceType == "tese") shaderOpts.stage = ShaderResourceOptions::ShaderStage::StageTessEval;
//		if (sourceType == "geom") shaderOpts.stage = ShaderResourceOptions::ShaderStage::StageGeom;
//		if (sourceType == "comp") shaderOpts.stage = ShaderResourceOptions::ShaderStage::StageComp;
//		shaderOpts.spirvBinary = Shader::CompileGLSLtoSPIRV(file_helpers::read_file_text(shaderOpts.sourceFilePath), shaderOpts.language, shaderOpts.stage);
//		Shader* s = new Shader(shaderOpts);
//		return s;
//	}
//
//	Log.Error("ResourceLoader", "Invalid resource filepath: " + resourceData.dataPath);
//	return nullptr;
//}

Resource* ResourceLoader::_loadExternalFile(const string filePath)
{
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
	Resource* res = nullptr;

	if (resourceType == "res") {
		res = EngineIO::ObjectLoader::LoadSerialisedResourceText(filePath);
		//The file is a standard resource, and can be converted directly to a resource
		loadedResources[filePath] = res;
	}

	// The file is external data, and must be encapsulated in a property resource
	bool presFileExists = file_helpers::file_exists(filePath + ".res");
	if (presFileExists) {
		res = EngineIO::ObjectLoader::LoadSerialisedResourceText(filePath);
	} else {

	}

	if (res == nullptr) {
		Log.Error("ResourceLoader", "Failed to load resource from filepath: " + filePath);
		return nullptr;
	}

	return res;
}

