#include "variant_type.h"
#include <cstring>

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

void Variant::CastTo(StoredType newType, bool castStrictly)
{
	switch (newType) {
		case StoredType::Empty:
		case StoredType::Void:
			Clear();
			break;
		case StoredType::Bool: {
			bool newB = _cast<bool>(_primitiveData, _currentType, castStrictly);
			Clear();
			_primitiveData._bool = newB;
			break;
		}
		case StoredType::Int: {
			int newI = _cast<int>(_primitiveData, _currentType, castStrictly);
			Clear();
			_primitiveData._int = newI;
			break;
		}
		case StoredType::UInt: {
			unsigned int newUI = _cast<unsigned int>(_primitiveData, _currentType, castStrictly);
			Clear();
			_primitiveData._uint = newUI;
			break;
		}
		case StoredType::LongLong: {
			long long newLL = _cast<long long>(_primitiveData, _currentType, castStrictly);
			Clear();
			_primitiveData._llong = newLL;
			break;
		}
		case StoredType::ULongLong: {
			unsigned long long newULL = _cast<unsigned long long>(_primitiveData, _currentType, castStrictly);
			Clear();
			_primitiveData._ullong = newULL;
			break;
		}
		case StoredType::Float: {
			float newF = _cast<float>(_primitiveData, _currentType, castStrictly);
			Clear();
			_primitiveData._float = newF;
			break;
		}
		case StoredType::Double: {
			double newD = _cast<double>(_primitiveData, _currentType, castStrictly);
			Clear();
			_primitiveData._double = newD;
			break;
		}
		case StoredType::String: {
			std::string newStr = _cast<std::string>(_primitiveData, _currentType, castStrictly);
			Clear();
			_primitiveData._stringPtr = &newStr;
			break;
		}
	}
	_currentType = newType;
}


std::string Variant::Stringify()
{
	return std::string();
}

Variant Variant::FromString(std::string str)
{
	return Variant();
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
	return (lhs.Type() == rhs.Type() && Variant::_same(lhs, rhs));
}

static bool operator!=(const Variant& lhs, const Variant& rhs) {
	return !operator==(lhs, rhs); 
}