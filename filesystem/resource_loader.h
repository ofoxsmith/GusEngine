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
	// A map containing pointers to all loaded resource instances.
	static std::unordered_map<string, Resource*> loadedResources;
	static Resource* _load(const string filePath);
	public:
	static void ImportResource(string filePath);
	static void Init();
	static void Cleanup();
	template <typename T>
	static T* Load(const string filePath) {
		Resource* res = _load(filePath);
		return dynamic_cast<T*>(res);
	};
	static Resource* Load(const string filePath) { return _load(filePath); }
};

