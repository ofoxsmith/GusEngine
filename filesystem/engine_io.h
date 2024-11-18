#pragma once
#include "core/types/object.h"
#include "core/types/resource.h"
namespace EngineIO {
	
	class ObjectSaver {
		static void SerialiseObjectBinary(Object res);
		static void SerialiseObjectText(Object res);
	};

	class ObjectLoader {
		static void LoadSerialisedObjectBinary();
		static void LoadSerialisedObjectText();
	};
}