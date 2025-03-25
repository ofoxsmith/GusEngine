#include "engine_io.h"
#include "core/types/object.h"

using namespace resources;
using namespace EngineIO;
void EngineIO::ObjectSaver::SerialiseResourceBinary(Resource res, std::string filepath)
{
	File outFile = EngineIO::FileSystem::OpenFile(filepath);

	string className = res._ClassName();
	string resourceName = res.Name();
	outFile.Write(className.c_str(), className.size());
	outFile.WriteByte(0x00);
	outFile.Write(resourceName.c_str(), resourceName.size());
	outFile.WriteByte(0x00);

	map<string, ObjectRTTIModel::ObjectPropertyDefinition> properties = res._GetPropertyList();
	map<string, ObjectRTTIModel::ObjectPropertyDefinition>::iterator it = properties.begin();

	while (it != properties.end()) {
		outFile.Write(it->first.c_str(), it->first.size());
		outFile.WriteByte(0x00);
		Variant value = res._Call(it->second.getterName);
		char* val = Variant::BinarySerialise(value);
		outFile.Write(val, Variant::BinarySerialisationLength(val));
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

Variant EngineIO::ObjectLoader::LoadBinaryVariant(File* input)
{
	short* typeBin = new short;
	input->Get((char*)typeBin, sizeof(short));
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
			input->Get(&valB, 1);
			return valB == 0xFF;
		case Variant::Int:
			input->Get((char*)&valInt, sizeof(int));
			return valInt;
		case Variant::UInt:
			input->Get((char*)&valUInt, sizeof(unsigned int));
			return valUInt;
		case Variant::LongLong:
			input->Get((char*)&valLLong, sizeof(long long));
			return valLLong;
		case Variant::ULongLong:
			input->Get((char*)&valULLong, sizeof(unsigned long long));
			return valULLong;
		case Variant::Float:
			input->Get((char*)&valFl, sizeof(float));
			return valFl;
		case Variant::Double:
			input->Get((char*)&valDb, sizeof(double));
			return valDb;
		case Variant::String:
			input->Get((char*)&strSize, sizeof(int));
			char* valStr = new char[strSize+1];
			input->Get(valStr, strSize);
			valStr[strSize] = 0x00;
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
		currentProp = "";
	}
	delete[] currentType;
	res->_Init();
	return res;
}

Resource* EngineIO::ObjectLoader::LoadSerialisedResourceText(std::string filepath)
{
	return nullptr;
}