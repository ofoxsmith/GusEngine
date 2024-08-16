#pragma once
#include "core/globals.h"
#include "project/resources/resource.h"
#include "file_helpers.h"
#include <unordered_map>

// Resource Types
#include "project/resources/shader.h"
using namespace resources;

class ResourceLoader
{
	private:
	static Shader* loadShaderFile(const string filePath);
	public:
	static Resource* Load(const string filePath);

};

