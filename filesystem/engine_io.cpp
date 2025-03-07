#include "engine_io.h"
#include "core/types/object.h"
#include <fstream>
#include <iostream>

using namespace resources;
void EngineIO::ObjectSaver::SerialiseResourceBinary(Resource res, std::string filepath)
{
    std::ofstream outFile(filepath, std::ios::binary);
	char nullByte = 0x00;

	string className = res._ClassName();
	string resourceName = res.Name();
	outFile.write(className.c_str(), className.size());
	outFile.write(&nullByte, 1);
	outFile.write(resourceName.c_str(), resourceName.size());
	outFile.write(&nullByte, 1);

	map<string, ObjectRTTIModel::ObjectPropertyDefinition> properties = res._GetPropertyList();
	map<string, ObjectRTTIModel::ObjectPropertyDefinition>::iterator it = properties.begin();

	while (it != properties.end()) {
		outFile.write(it->first.c_str(), it->first.size());
		outFile.write(&nullByte, 1);
		Variant value = res._Call(it->second.getterName);
		char* val = Variant::BinarySerialise(value);
		outFile.write(val, Variant::BinarySerialisationLength(val));
		delete val;
		it++;
	}

}

void EngineIO::ObjectSaver::SerialiseResourceText(Resource res, std::string filepath)
{
}

Resource* EngineIO::ObjectLoader::LoadSerialisedResourceBinary(std::string filepath)
{
	return nullptr;
}

Resource* EngineIO::ObjectLoader::LoadSerialisedResourceText(std::string filepath)
{
	return nullptr;
}
