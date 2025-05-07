#pragma once
#include "core/globals.h"
#include "core/types/resource.h"
#include <unordered_map>

// Resource Types
#include "project/resources/shader.h"
using namespace resources;


class ResourceLoader
{
	private:
	struct ImportedResource {
		string location;
		string cacheLocation;
		string hash;
	};

	// A map containing pointers to all loaded resource instances.
	static std::unordered_map<string, Resource*> loadedResources;
	static std::unordered_map<string, ImportedResource> projectResources;
	static Resource* _load(const string filePath);
	public:
	enum class ImportResult {
		IMPORTED,
		IMPORT_FAIL,
		NOT_RECOGNISED
	};
	static void Init();
	static bool IsResourceImported(string filePath);
	static bool HasImportCacheChanged(string filePath);
	static ImportResult ImportResource(string filePath);
	static void Cleanup() {};


	template <typename T>
	static T* Load(const string filePath) {
		Resource* res = _load(filePath);
		return dynamic_cast<T*>(res);
	};
	static Resource* Load(const string filePath) { return _load(filePath); }
};

