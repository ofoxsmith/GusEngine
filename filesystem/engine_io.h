#pragma once
#include "core/types/object.h"
#include "core/types/resource.h"
using namespace resources;
namespace EngineIO {
	
	class ObjectSaver {
		public:
		static void SerialiseObjectBinary(Object res, std::string filepath);
		static void SerialiseObjectText(Object res, std::string filepath);
	};

	class ObjectLoader {
		public:
		static Resource* LoadSerialisedObjectBinary(std::string filepath);
		static Resource* LoadSerialisedObjectText(std::string filepath);
	};
}