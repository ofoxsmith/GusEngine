#include "variant_type.h"
#include <cstring>

bool Variant::_same(const Variant& v1, const Variant& v2)
{
	if (v1.Type() != v2.Type()) return false;
	switch (v1.Type()) {
		case Empty:
		case Void:
			return true;
		case Bool:
			return v1.Value<bool>() == v2.Value<bool>();
		case Int:
			return v1.Value<int>() == v2.Value<int>();
		case UInt:
			return v1.Value<unsigned int>() == v2.Value<unsigned int>();
		case LongLong:
			return v1.Value<long long>() == v2.Value<long long>();
		case ULongLong:
			return v1.Value<unsigned long long>() == v2.Value<unsigned long long>();
		case Float:
			return v1.Value<float>() == v2.Value<float>();
		case Double:
			return v1.Value<double>() == v2.Value<double>();
		case String:
			return v1.Value<std::string>() == v2.Value<std::string>();
	}
	return false;
}

void Variant::CastTo(StoredType newType)
{
	switch (newType) {
		case Empty:
		case Void:
			Clear();
			break;
		case Bool: {
			bool newB = _weakCast<bool>(_primitiveData, _currentType);
			Clear();
			_primitiveData._bool = newB;
			break;
		}
		case Int: {
			int newI = _weakCast<int>(_primitiveData, _currentType);
			Clear();
			_primitiveData._int = newI;
			break;
		}
		case UInt: {
			unsigned int newUI = _weakCast<unsigned int>(_primitiveData, _currentType);
			Clear();
			_primitiveData._uint = newUI;
			break;
		}
		case LongLong: {
			long long newLL = _weakCast<long long>(_primitiveData, _currentType);
			Clear();
			_primitiveData._llong = newLL;
			break;
		}
		case ULongLong: {
			unsigned long long newULL = _weakCast<unsigned long long>(_primitiveData, _currentType);
			Clear();
			_primitiveData._ullong = newULL;
			break;
		}
		case Float: {
			float newF = _weakCast<float>(_primitiveData, _currentType);
			Clear();
			_primitiveData._float = newF;
			break;
		}
		case Double: {
			double newD = _weakCast<double>(_primitiveData, _currentType);
			Clear();
			_primitiveData._double = newD;
			break;
		}
		case String: {
			std::string newStr = _weakCast<std::string>(_primitiveData, _currentType);
			Clear();
			_primitiveData._stringPtr = &newStr;
			break;
		}
	}
	_currentType = newType;
}

void Variant::CastToStrict(StoredType newType) {
	switch (newType) {
		case Empty:
		case Void:
			Clear();
			break;
		case Bool: {
			bool newB = _strictCast<bool>(_primitiveData, _currentType);
			Clear();
			_primitiveData._bool = newB;
			break;
		}
		case Int: {
			int newI = _strictCast<int>(_primitiveData, _currentType);
			Clear();
			_primitiveData._int = newI;
			break;
		}
		case UInt: {
			unsigned int newUI = _strictCast<unsigned int>(_primitiveData, _currentType);
			Clear();
			_primitiveData._uint = newUI;
			break;
		}
		case LongLong: {
			long long newLL = _strictCast<long long>(_primitiveData, _currentType);
			Clear();
			_primitiveData._llong = newLL;
			break;
		}
		case ULongLong: {
			unsigned long long newULL = _strictCast<unsigned long long>(_primitiveData, _currentType);
			Clear();
			_primitiveData._ullong = newULL;
			break;
		}
		case Float: {
			float newF = _strictCast<float>(_primitiveData, _currentType);
			Clear();
			_primitiveData._float = newF;
			break;
		}
		case Double: {
			double newD = _strictCast<double>(_primitiveData, _currentType);
			Clear();
			_primitiveData._double = newD;
			break;
		}
		case String: {
			std::string newStr = _strictCast<std::string>(_primitiveData, _currentType);
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
		_currentType = String;
		_primitiveData._stringPtr = new std::string(*other._primitiveData._stringPtr);
	}
	else {
		_currentType = other._currentType;
		_primitiveData = other._primitiveData;
	}
}

Variant::Variant(Variant&& other) noexcept : Variant() {
	_swap(*this, other);
}

static bool operator==(const Variant& lhs, const Variant& rhs) {
	return (lhs.Type() == rhs.Type() && Variant::_same(lhs, rhs));
}

static bool operator!=(const Variant& lhs, const Variant& rhs) {
	return !operator==(lhs, rhs); 
}