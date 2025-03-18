#include "resource_loader.h"
#include "file_helpers.h"
#include "engine_io.h"
void ResourceLoader::Init() {
	//dataCache.Init();
}

void ResourceLoader::Cleanup() {
	//dataCache.Cleanup();
}

std::unordered_map<string, Resource*> ResourceLoader::loadedResources;

// Loads a resource from the filesystem
Resource* ResourceLoader::_load(const string filePath) {
	string resourceType = file_helpers::get_file_type(filePath);

	if (loadedResources.contains(filePath)) {
		return loadedResources[filePath];
	}

	if (dataCache.IsResourceImported(filePath)) {
		loadedResources[filePath]
	}

	if (!file_helpers::file_exists(filePath)) {
		Log.Error("ResourceLoader", filePath + " does not exist.");
		return nullptr;
	}

	Resource* res = nullptr;

	if (resourceType == "res") {
		res = EngineIO::ObjectLoader::LoadSerialisedResourceText(filePath);
		//The file is a standard resource, and can be converted directly to a resource
		loadedResources[filePath] = res;
	}

	if (res == nullptr) {
		Log.Error("ResourceLoader", "Failed to load resource from filepath: " + filePath);
		return nullptr;
	}

	return res;
}

