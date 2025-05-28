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
		case StoredType::Int32:
			return v1.Value<int32_t>() == v2.Value<int32_t>();
		case StoredType::UInt32:
			return v1.Value<uint32_t>() == v2.Value<uint32_t>();
		case StoredType::Int64:
			return v1.Value<int64_t>() == v2.Value<int64_t>();
		case StoredType::UInt64:
			return v1.Value<uint64_t>() == v2.Value<uint64_t>();
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
			break;
		case StoredType::Bool: {
			bool newB = operator bool();
			_primitiveData._bool = newB;
			break;
		}
		case StoredType::Int32: {
			int32_t newI = operator int32_t();
			_primitiveData._int = newI;
			break;
		}
		case StoredType::UInt32: {
			uint32_t newUI = operator uint32_t();
			_primitiveData._uint = newUI;
			break;
		}
		case StoredType::Int64: {
			int64_t newLL = operator int64_t();
			_primitiveData._llong = newLL;
			break;
		}
		case StoredType::UInt64: {
			uint64_t newULL = operator uint64_t();
			_primitiveData._ullong = newULL;
			break;
		}
		case StoredType::Float: {
			float newF = operator float();
			_primitiveData._float = newF;
			break;
		}
		case StoredType::Double: {
			double newD = operator double();
			_primitiveData._double = newD;
			break;
		}
		case StoredType::String: {
			std::string newStr = operator string();
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
		case StoredType::Int32:
			buffer = new char[VARIANT_ENUM_SIZE + sizeof(int32_t)];
			memcpy(buffer, &typeVal, VARIANT_ENUM_SIZE);
			memcpy(buffer + VARIANT_ENUM_SIZE, &v._primitiveData, sizeof(int32_t));
			return buffer;
		case StoredType::UInt32:
			buffer = new char[VARIANT_ENUM_SIZE + sizeof(uint32_t)];
			memcpy(buffer, &typeVal, VARIANT_ENUM_SIZE);
			memcpy(buffer + VARIANT_ENUM_SIZE, &v._primitiveData, sizeof(uint32_t));
			return buffer;
		case StoredType::Int64:
			buffer = new char[VARIANT_ENUM_SIZE + sizeof(int64_t)];
			memcpy(buffer, &typeVal, VARIANT_ENUM_SIZE);
			memcpy(buffer + VARIANT_ENUM_SIZE, &v._primitiveData, sizeof(int64_t));
			return buffer;
		case StoredType::UInt64:
			buffer = new char[VARIANT_ENUM_SIZE + sizeof(uint64_t)];
			memcpy(buffer, &typeVal, VARIANT_ENUM_SIZE);
			memcpy(buffer + VARIANT_ENUM_SIZE, &v._primitiveData, sizeof(uint64_t));
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
			int32_t size = static_cast<int32_t>(v._primitiveData._stringPtr->size());
			buffer = new char[VARIANT_ENUM_SIZE + sizeof(int32_t) + size];
			memcpy(buffer, &typeVal, VARIANT_ENUM_SIZE);
			memcpy(buffer + VARIANT_ENUM_SIZE, &size, sizeof(int32_t));
			memcpy(buffer + VARIANT_ENUM_SIZE + sizeof(int32_t), v._primitiveData._stringPtr->data(), size);
			return buffer;
	}
	throw runtime_error("");
}

int32_t Variant::BinarySerialisationLength(char* bin) {
	short* typeBin = new short;
	memcpy(typeBin, bin, VARIANT_ENUM_SIZE);
	Variant::StoredType type = static_cast<Variant::StoredType>(*typeBin);
	delete typeBin;
	int32_t len = 0;
	switch (type) {
		case StoredType::Empty:
		case StoredType::Void:
			len = 0;
			break;
		case StoredType::Bool:
			len = sizeof(char);
			break;
		case StoredType::Int32:
			len = sizeof(int32_t);
			break;
		case StoredType::UInt32:
			len = sizeof(uint32_t);
			break;
		case StoredType::Int64:
			len = sizeof(int64_t);
			break;
		case StoredType::UInt64:
			len = sizeof(uint64_t);
			break;
		case StoredType::Float:
			len = sizeof(float);
			break;
		case StoredType::Double:
			len = sizeof(double);
			break;
		case StoredType::String:
			int32_t size = 0;
			memcpy(&size, bin + VARIANT_ENUM_SIZE, sizeof(int32_t));
			len = sizeof(int32_t) + sizeof(char) * size;
			break;
	}
	return len + VARIANT_ENUM_SIZE;
}


std::string Variant::StringSerialise(Variant v)
{
	switch (v.Type()) {
		case StoredType::Empty:
		case StoredType::Void:
			return "null";
		case StoredType::Bool:
			return v.Value<bool>() ? "true" : "false";
		case StoredType::Int32:
			return "Int32" + std::to_string(v.Value<int32_t>());
		case StoredType::UInt32:
			return "UInt32" + std::to_string(v.Value<uint32_t>());
		case StoredType::Int64:
			return "Llong" + std::to_string(v.Value<int64_t>());
		case StoredType::UInt64:
			return "ULLong" + std::to_string(v.Value<uint64_t>());
		case StoredType::Float:
			return "Float" + std::to_string(v.Value<float>());
		case StoredType::Double:
			return "Double" + std::to_string(v.Value<double>());
		case StoredType::String:
			return "\"" + v.Value<string>() + "\"";
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
	else if (type == "Int32") {
		return Variant(std::stoi(content));
	}
	else if (type == "UInt32") {
		return Variant(static_cast<uint32_t>(std::stoul(content)));
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

Variant::Variant(int32_t data) {
	_primitiveData._int = data; 
	_currentType = StoredType::Int32;
}

Variant::Variant(uint32_t data) {
	_primitiveData._uint = data; 
	_currentType = StoredType::UInt32;
}

Variant::Variant(int64_t data) {
	_primitiveData._llong = data; 
	_currentType = StoredType::Int64;
}

Variant::Variant(uint64_t data) {
	_primitiveData._ullong = data; 
	_currentType = StoredType::UInt64;
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

static bool operator==(const Variant& lhs, const Variant& rhs) {
	return (Variant::_same(lhs, rhs));
}

static bool operator!=(const Variant& lhs, const Variant& rhs) {
	return !operator==(lhs, rhs); 
}