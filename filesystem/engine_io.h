#pragma once
#include "core/types/object.h"
#include "core/types/resource.h"
using namespace resources;
namespace EngineIO {
	
	class ObjectSaver {
		public:
		static void SerialiseResourceBinary(Resource res, std::string filepath);
		static void SerialiseResourceText(Resource res, std::string filepath);
	};

	class ObjectLoader {
		public:
		static Resource* LoadSerialisedResourceBinary(std::string filepath);
		static Resource* LoadSerialisedResourceText(std::string filepath);
	};
}