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
	std::ofstream outFile(filepath);
	outFile << "[" << res._ClassName() << "] " << res.Name()  << std::endl;

	map<string, ObjectRTTIModel::ObjectPropertyDefinition> properties = res._GetPropertyList();
	map<string, ObjectRTTIModel::ObjectPropertyDefinition>::iterator it = properties.begin();

	while (it != properties.end()) {
		Variant value = res._Call(it->second.getterName);
		outFile << it->first << ": " << Variant::StringSerialise(value) << "" << std::endl;
		it++;
	}

}

Variant EngineIO::ObjectLoader::LoadBinaryVariant(std::ifstream* input)
{
	short* typeBin = new short;
	input->read((char*)typeBin, sizeof(short));
	Variant::StoredType type = static_cast<Variant::StoredType>(*typeBin);
	delete typeBin;

	char valB = 0;
	int valInt = 0;
	unsigned int valUInt = 0;
	long long valLLong = 0;
	unsigned long long valULLong = 0;
	float valFl = 0;
	double valDb = 0;
	int strSize = 0;

	switch (type) {
		case Variant::Empty:
		case Variant::Void:
			return Variant(Variant::Void);
		case Variant::Bool:
			input->read(&valB, 1);
			return valB == 0xFF;
		case Variant::Int:
			input->read((char*)&valInt, sizeof(int));
			return valInt;
		case Variant::UInt:
			input->read((char*)&valUInt, sizeof(unsigned int));
			return valUInt;
		case Variant::LongLong:
			input->read((char*)&valLLong, sizeof(long long));
			return valLLong;
		case Variant::ULongLong:
			input->read((char*)&valULLong, sizeof(unsigned long long));
			return valULLong;
		case Variant::Float:
			input->read((char*)&valFl, sizeof(float));
			return valFl;
		case Variant::Double:
			input->read((char*)&valDb, sizeof(double));
			return valDb;
		case Variant::String:
			input->read((char*)&strSize, sizeof(int));
			char* valStr = new char[strSize];
			input->read(valStr, strSize);
			return std::string(valStr);
	}
	return Variant();
}

Resource* EngineIO::ObjectLoader::LoadSerialisedResourceBinary(std::string filepath)
{
	std::ifstream inFile(filepath, std::ios::binary);

	if (!inFile.is_open()) {
		Log.Error("EngineIO", "Cannot load file " + filepath);
	}

	std::string type;
	std::string name;
	
	char ch;
	while (inFile.read(&ch, 1)) {
		if (ch == '\0') {
			break;
		}
		type += ch; 
	}
	while (inFile.read(&ch, 1)) {
		if (ch == '\0') {
			break;
		}
		name += ch;
	}

	engine_type_registry::EngineClass engCls = engine_type_registry::type_registry::_registered_classes[type];
	Resource* res = dynamic_cast<Resource*>((*engCls._dynamic_constructor)());

	std::string currentProp;
	char* currentType = new char[VARIANT_ENUM_SIZE];
	while (!inFile.eof()) {
		while (inFile.read(&ch, 1)) {
			if (ch == '\0') {
				break;
			}
			currentProp += ch;
		}

		res->_Set(currentProp, LoadBinaryVariant(&inFile));
	}
	delete[] currentType;
	res->_Init();
	return res;
}

Resource* EngineIO::ObjectLoader::LoadSerialisedResourceText(std::string filepath)
{
	return nullptr;
}
