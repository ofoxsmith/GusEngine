#include "variant_type.h"

#include <cstring>
#include <regex>

bool Variant::_same(const Variant& v1, const Variant& v2)
{
	if (v1.Type() != v2.Type()) return false;
	switch (v1.Type()) {
		case StoredType::Empty:
		case StoredType::Void:
			return true;
		case StoredType::Bool:
			return v1.Value<bool>() == v2.Value<bool>();
		case StoredType::Int:
			return v1.Value<int>() == v2.Value<int>();
		case StoredType::UInt:
			return v1.Value<unsigned int>() == v2.Value<unsigned int>();
		case StoredType::LongLong:
			return v1.Value<long long>() == v2.Value<long long>();
		case StoredType::ULongLong:
			return v1.Value<unsigned long long>() == v2.Value<unsigned long long>();
		case StoredType::Float:
			return v1.Value<float>() == v2.Value<float>();
		case StoredType::Double:
			return v1.Value<double>() == v2.Value<double>();
		case StoredType::String:
			return v1.Value<std::string>() == v2.Value<std::string>();
	}
	return false;
}

void Variant::CastTo(StoredType newType)
{
	switch (newType) {
		case StoredType::Empty:
		case StoredType::Void:
			Clear();
			break;
		case StoredType::Bool: {
			bool newB = operator bool();
			Clear();
			_primitiveData._bool = newB;
			break;
		}
		case StoredType::Int: {
			int newI = operator int();
			Clear();
			_primitiveData._int = newI;
			break;
		}
		case StoredType::UInt: {
			unsigned int newUI = operator unsigned int();
			Clear();
			_primitiveData._uint = newUI;
			break;
		}
		case StoredType::LongLong: {
			long long newLL = operator long long();
			Clear();
			_primitiveData._llong = newLL;
			break;
		}
		case StoredType::ULongLong: {
			unsigned long long newULL = operator unsigned long long();
			Clear();
			_primitiveData._ullong = newULL;
			break;
		}
		case StoredType::Float: {
			float newF = operator float();
			Clear();
			_primitiveData._float = newF;
			break;
		}
		case StoredType::Double: {
			double newD = operator double();
			Clear();
			_primitiveData._double = newD;
			break;
		}
		case StoredType::String: {
			std::string newStr = operator string();
			Clear();
			_primitiveData._stringPtr = &newStr;
			break;
		}
	}
	_currentType = newType;
}

char* Variant::BinarySerialise(Variant v)
{
	char* buffer;
	short typeVal = static_cast<short>(v._currentType);
	switch (v.Type()) {
		case StoredType::Empty:
		case StoredType::Void:
			buffer = new char[VARIANT_ENUM_SIZE];
			memcpy(buffer, &typeVal, VARIANT_ENUM_SIZE);
			buffer[0] = static_cast<char>(v.Type());
			return buffer;
		case StoredType::Bool:
			buffer = new char[VARIANT_ENUM_SIZE + 1];
			memcpy(buffer, &typeVal, VARIANT_ENUM_SIZE);
			buffer[VARIANT_ENUM_SIZE] = v.Value<bool>() ? 0xFF : 0x00;
			return buffer;
		case StoredType::Int:
			buffer = new char[VARIANT_ENUM_SIZE + sizeof(int)];
			memcpy(buffer, &typeVal, VARIANT_ENUM_SIZE);
			memcpy(buffer + VARIANT_ENUM_SIZE, &v._primitiveData, sizeof(int));
			return buffer;
		case StoredType::UInt:
			buffer = new char[VARIANT_ENUM_SIZE + sizeof(unsigned int)];
			memcpy(buffer, &typeVal, VARIANT_ENUM_SIZE);
			memcpy(buffer + VARIANT_ENUM_SIZE, &v._primitiveData, sizeof(unsigned int));
			return buffer;
		case StoredType::LongLong:
			buffer = new char[VARIANT_ENUM_SIZE + sizeof(long long)];
			memcpy(buffer, &typeVal, VARIANT_ENUM_SIZE);
			memcpy(buffer + VARIANT_ENUM_SIZE, &v._primitiveData, sizeof(long long));
			return buffer;
		case StoredType::ULongLong:
			buffer = new char[VARIANT_ENUM_SIZE + sizeof(unsigned long long)];
			memcpy(buffer, &typeVal, VARIANT_ENUM_SIZE);
			memcpy(buffer + VARIANT_ENUM_SIZE, &v._primitiveData, sizeof(unsigned long long));
			return buffer;
		case StoredType::Float:
			buffer = new char[VARIANT_ENUM_SIZE + sizeof(float)];
			memcpy(buffer, &typeVal, VARIANT_ENUM_SIZE);
			memcpy(buffer + VARIANT_ENUM_SIZE, &v._primitiveData, sizeof(float));
			return buffer;
		case StoredType::Double:
			buffer = new char[VARIANT_ENUM_SIZE + sizeof(double)];
			memcpy(buffer, &typeVal, VARIANT_ENUM_SIZE);
			memcpy(buffer + VARIANT_ENUM_SIZE, &v._primitiveData, sizeof(double));
			return buffer;
		case StoredType::String:
			int size = static_cast<int>(v._primitiveData._stringPtr->size());
			buffer = new char[VARIANT_ENUM_SIZE + sizeof(int) + size];
			memcpy(buffer, &typeVal, VARIANT_ENUM_SIZE);
			memcpy(buffer + VARIANT_ENUM_SIZE, &size, sizeof(int));
			memcpy(buffer + VARIANT_ENUM_SIZE + sizeof(int), v._primitiveData._stringPtr->data(), size);
			return buffer;
	}
	throw runtime_error("");
}

int Variant::BinarySerialisationLength(char* bin) {
	short* typeBin = new short;
	memcpy(typeBin, bin, VARIANT_ENUM_SIZE);
	Variant::StoredType type = static_cast<Variant::StoredType>(*typeBin);
	delete typeBin;
	int len = 0;
	switch (type) {
		case StoredType::Empty:
		case StoredType::Void:
			len = 0;
			break;
		case StoredType::Bool:
			len = sizeof(char);
			break;
		case StoredType::Int:
			len = sizeof(int);
			break;
		case StoredType::UInt:
			len = sizeof(unsigned int);
			break;
		case StoredType::LongLong:
			len = sizeof(long long);
			break;
		case StoredType::ULongLong:
			len = sizeof(unsigned long long);
			break;
		case StoredType::Float:
			len = sizeof(float);
			break;
		case StoredType::Double:
			len = sizeof(double);
			break;
		case StoredType::String:
			int size = 0;
			memcpy(&size, bin + VARIANT_ENUM_SIZE, sizeof(int));
			len = sizeof(int) + sizeof(char) * size;
			break;
	}
	return len + VARIANT_ENUM_SIZE;
}

Variant Variant::FromBinary(char* bin)
{
	short* typeBin = new short;
	memcpy(typeBin, bin, VARIANT_ENUM_SIZE);
	Variant::StoredType type = static_cast<Variant::StoredType>(*typeBin);
	delete typeBin;

	int valInt = 0;
	unsigned int valUInt = 0;
	long long valLLong = 0;
	unsigned long long valULLong = 0;
	float valFl = 0;
	double valDb = 0;
	int strSize = 0;

	switch (type) {
		case StoredType::Empty:
		case StoredType::Void:
			return Variant(Variant::Void);
		case StoredType::Bool:
			return *(bin + VARIANT_ENUM_SIZE) == 0xFF;
		case StoredType::Int:
			memcpy(&valInt, bin + VARIANT_ENUM_SIZE, sizeof(int));
			return valInt;
		case StoredType::UInt:
			memcpy(&valUInt, bin + VARIANT_ENUM_SIZE, sizeof(unsigned int));
			return valUInt;
		case StoredType::LongLong:
			memcpy(&valLLong, bin + VARIANT_ENUM_SIZE, sizeof(long long));
			return valLLong;
		case StoredType::ULongLong:
			memcpy(&valULLong, bin + VARIANT_ENUM_SIZE, sizeof(unsigned long long));
			return valULLong;
		case StoredType::Float:
			memcpy(&valFl, bin + VARIANT_ENUM_SIZE, sizeof(float));
			return valFl;
		case StoredType::Double:
			memcpy(&valDb, bin + VARIANT_ENUM_SIZE, sizeof(double));
			return valDb;
		case StoredType::String:
			memcpy(&strSize, bin + VARIANT_ENUM_SIZE, sizeof(int));

			char* valStr = new char[strSize];
			memcpy(valStr, bin + VARIANT_ENUM_SIZE + sizeof(int), sizeof(char) * strSize);
			return std::string(valStr);
	}
	return Variant();
}

std::string Variant::StringSerialise(Variant v)
{
	switch (v.Type()) {
		case StoredType::Empty:
		case StoredType::Void:
			return ";";
		case StoredType::Bool:
			return v.Value<bool>() ? "Bool1" : "Bool0";
		case StoredType::Int:
			return "Int" + std::to_string(v.Value<int>());
		case StoredType::UInt:
			return "UInt" + std::to_string(v.Value<unsigned int>());
		case StoredType::LongLong:
			return "LLong" + std::to_string(v.Value<long long>());
		case StoredType::ULongLong:
			return "ULLong" + std::to_string(v.Value<unsigned long long>());
		case StoredType::Float:
			return "Float" + std::to_string(v.Value<float>());
		case StoredType::Double:
			return "Double" + std::to_string(v.Value<double>());
		case StoredType::String:
			return "String[" + v.Value<string>() + "];";
	}
	return "";
}

Variant Variant::FromString(std::string* str)
{
	std::regex pattern (R"(^([A-Za-z]+)(.+);$)");
	std::smatch match;

	if (!std::regex_search(*str, match, pattern)) {
		return Variant(Variant::Empty);
	}

	string type = match[1];
	string content = match[2];

	if ((*str).starts_with(";")) {
		return Variant(Variant::Void);
	}
	else if (type == "Bool") {
		return Variant(content == "1" ? true : false);
	}
	else if (type == "Int") {
		return Variant(std::stoi(content));
	}
	else if (type == "UInt") {
		return Variant(static_cast<unsigned int>(std::stoul(content)));
	}
	else if (type == "LLong") {
		return Variant(std::stoll(content));
	}
	else if (type == "ULLong") {
		return Variant(std::stoull(content));
	}
	else if (type == "Float") {
		return Variant(std::stof(content));
	}
	else if (type == "Double") {
		return Variant(std::stod(content));
	}
	else if (type == "String") {
		content.pop_back();
		return Variant(content.substr(1));
	}

	return Variant(Variant::Empty);
}


Variant::Variant(bool data) {
	_primitiveData._bool = data; 
	_currentType = StoredType::Bool;
}

Variant::Variant(int data) {
	_primitiveData._int = data; 
	_currentType = StoredType::Int;
}

Variant::Variant(unsigned int data) {
	_primitiveData._uint = data; 
	_currentType = StoredType::UInt;
}

Variant::Variant(long long data) {
	_primitiveData._llong = data; 
	_currentType = StoredType::LongLong;
}

Variant::Variant(unsigned long long data) {
	_primitiveData._ullong = data; 
	_currentType = StoredType::ULongLong;
}

Variant::Variant(float data) {
	_primitiveData._float = data; 
	_currentType = StoredType::Float;
}

Variant::Variant(double data) {
	_primitiveData._double = data; 
	_currentType = StoredType::Double;
}

Variant::Variant(std::string data)
{
	_currentType = StoredType::String;
	_primitiveData._stringPtr = new std::string(data);
}

Variant::Variant(const Variant& other)
{
	if (other._currentType & StoredType::String) {
		_currentType = StoredType::String;
		_primitiveData._stringPtr = new std::string(*other._primitiveData._stringPtr);
	}
	else {
		_currentType = other._currentType;
		_primitiveData = other._primitiveData;
	}
}

static bool operator==(const Variant& lhs, const Variant& rhs) {
	return (Variant::_same(lhs, rhs));
}

static bool operator!=(const Variant& lhs, const Variant& rhs) {
	return !operator==(lhs, rhs); 
}