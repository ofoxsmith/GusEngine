#pragma once
#include "core/globals.h"
#include "project/resources/resource.h"
#include <unordered_map>

// Resource Types
#include "project/resources/shader.h"
using namespace resources;

class ResourceLoader
{
	private:
	static Shader* loadShaderFile(const string filePath);
	static Resource* _load(const string filePath);
	public:
	template <typename T>
	static T* Load(const string filePath) {
		Resource* res = _load(filePath);
		return dynamic_cast<T*>(res);
	};
	static Resource* Load(const string filePath) { return _load(filePath); }
};

