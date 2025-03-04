#pragma once
#include <string>
#include "core/globals.h"
#include <concepts>
#include <type_traits>

template <typename T>
concept IsIntegerEnum = (std::is_enum<T>::value) && (std::is_same_v<std::underlying_type_t<T>, int>);


// Implementation of a variant data type, which can dynamically hold primitve data types, used to correctly cast data for use with RTTI.
struct Variant {
	enum StoredType: short {
		// NOTE: Empty and Void represent two different types - A variant that has not been assigned, vs an assigned variant holding nothing.
		Empty = 0,
		Void = (1u << 0),

		// Primitive types:
		Bool = (1u << 1),
		Int = (1u << 2),
		UInt = (1u << 3),
		LongLong = (1u << 4),
		ULongLong = (1u << 5),
		Float = (1u << 6),
		Double = (1u << 7),

		// Complex types:
		String = (1u << 8),
		// WIP

	};

	inline static string VariantTypeToString(StoredType type) {
		switch (type) {
			case StoredType::Empty:
				return "Empty";
			case StoredType::Void:
				return "Void";
			case StoredType::Bool:
				return "Bool";
			case StoredType::Int:
				return "Int";
			case StoredType::UInt:
				return "UInt";
			case StoredType::LongLong:
				return "LongLong";
			case StoredType::ULongLong:
				return "ULongLong";
			case StoredType::Float:
				return "Float";
			case StoredType::Double:
				return "Double";
			case StoredType::String:
				return "String";
			default:
				return "";
		}
	}

	private:

	union variantData {
		bool _bool;
		int _int;
		unsigned int _uint;
		long long _llong;
		unsigned long long _ullong;
		float _float;
		double _double;

		std::string* _stringPtr;
	} _primitiveData;

	StoredType _currentType;

	friend void _swap(Variant& first, Variant& second) noexcept
	{
		using std::swap;
		swap(first._primitiveData, second._primitiveData);
		swap(first._currentType, first._currentType);
	}

	static bool _same(const Variant& v1, const Variant& v2);

	public:
	StoredType Type() const { return _currentType; }

	void Clear() {
		if (_currentType == StoredType::String) {
			delete _primitiveData._stringPtr;
		}
		_currentType = StoredType::Empty;
	};

	template <typename T>
	T Value() const {
		return operator T();
	};

	// Returns true if the value seems to be false like or 0 like.
	bool IsValueFalseLike() inline const {
		if (IsEmptyOrVoid()) return true;
		switch (_currentType) {
			case StoredType::String:
				return (_primitiveData._stringPtr)->empty();
			default:
				return operator bool() == false;
		}
	}


	bool IsEmptyOrVoid() inline const {
		return _currentType & (StoredType::Void | StoredType::Empty);
	}

	bool IsInt() inline const {
		return (_currentType & (StoredType::Int | StoredType::UInt | StoredType::LongLong | StoredType::ULongLong));
	};

	bool IsNumeric() inline const {
		return (_currentType & (StoredType::Int | StoredType::UInt | StoredType::LongLong | StoredType::ULongLong | StoredType::Float | StoredType::Double));
	};

	bool IsSignedInt() inline const {
		return (_currentType & (StoredType::Int | StoredType::LongLong));
	};

	void CastTo(StoredType newType);
	static char* BinarySerialise(Variant v);
	static Variant FromBinary(char* bin);
	static std::string StringSerialise(Variant v);
	static Variant FromString(std::string str);
	
	//// Operators and constructors

	Variant() { 
		_primitiveData._int = 0; 
		_currentType = StoredType::Empty; 
	};
	Variant(StoredType type) {
		_primitiveData._int = 0;
		_currentType = type;
	}
	Variant(const Variant& other);
	Variant(Variant&& other) noexcept : Variant() {
		_swap(*this, other);
	}
	~Variant() {
		Clear();
	}

	Variant& operator=(Variant other) {
		_swap(*this, other);
		return *this;
	}


	Variant(bool data);
	Variant(int data);
	Variant(unsigned int data);
	Variant(long long data);
	Variant(unsigned long long data);
	Variant(float data);
	Variant(double data);
	Variant(std::string data);

	operator bool() const {
		switch (_currentType) {
			case StoredType::Empty:
			case StoredType::Void:
				return false;
			case StoredType::Bool:
				return _primitiveData._bool;
			case StoredType::Int:
				return _primitiveData._int;
			case StoredType::UInt:
				return _primitiveData._uint;
			case StoredType::LongLong:
				return _primitiveData._llong;
			case StoredType::ULongLong:
				return _primitiveData._ullong;
			case StoredType::Float:
				return _primitiveData._float;
			case StoredType::Double:
				return _primitiveData._double;
		}
		return false;
	};
	operator int() const {
		switch (_currentType) {
			case StoredType::Empty:
			case StoredType::Void:
				return 0;
			case StoredType::LongLong:
				return static_cast<int>(_primitiveData._llong);
			case StoredType::ULongLong:
				return static_cast<int>(_primitiveData._ullong);
			case StoredType::Float:
				return static_cast<int>(_primitiveData._float);
			case StoredType::Double:
				return static_cast<int>(_primitiveData._double);
			case StoredType::Bool:
				return _primitiveData._bool;
			case StoredType::Int:
				return _primitiveData._int;
			case StoredType::UInt:
				return _primitiveData._uint;
		}
		return 0;
	};
	operator unsigned int() const {
		switch (_currentType) {
			case StoredType::Empty:
			case StoredType::Void:
				return 0;
			case StoredType::Bool:
				return _primitiveData._bool;
			case StoredType::Int:
				return _primitiveData._int;
			case StoredType::UInt:
				return _primitiveData._uint;
			case StoredType::LongLong:
				return static_cast<unsigned int>(_primitiveData._llong);
			case StoredType::ULongLong:
				return static_cast<unsigned int>(_primitiveData._ullong);
			case StoredType::Float:
				return static_cast<unsigned int>(_primitiveData._float);
			case StoredType::Double:
				return static_cast<unsigned int>(_primitiveData._double);
		}
		return 0;
	};

	operator long long() const {
		switch (_currentType) {
			case StoredType::Empty:
			case StoredType::Void:
				return 0;
			case StoredType::Bool:
				return _primitiveData._bool;
			case StoredType::Int:
				return _primitiveData._int;
			case StoredType::UInt:
				return _primitiveData._uint;
			case StoredType::LongLong:
				return _primitiveData._llong;
			case StoredType::ULongLong:
				return _primitiveData._ullong;
			case StoredType::Float:
				return static_cast<long long>(_primitiveData._float);
			case StoredType::Double:
				return static_cast<long long>(_primitiveData._double);
		}
		return 0;

	};
	operator unsigned long long() const {
		switch (_currentType) {
			case StoredType::Empty:
			case StoredType::Void:
				return 0;
			case StoredType::Bool:
				return _primitiveData._bool;
			case StoredType::Int:
				return _primitiveData._int;
			case StoredType::UInt:
				return _primitiveData._uint;
			case StoredType::LongLong:
				return _primitiveData._llong;
			case StoredType::ULongLong:
				return _primitiveData._ullong;
			case StoredType::Float:
				return static_cast<unsigned long long>(_primitiveData._float);
			case StoredType::Double:
				return static_cast<unsigned long long>(_primitiveData._double);
		}
		return 0;
	}
	operator float() const {
		switch (_currentType) {
			case StoredType::Empty:
			case StoredType::Void:
				return 0;
			case StoredType::Bool:
				return _primitiveData._bool;
			case StoredType::Int:
				return static_cast<float>(_primitiveData._int);
			case StoredType::UInt:
				return static_cast<float>(_primitiveData._uint);
			case StoredType::LongLong:
				return static_cast<float>(_primitiveData._llong);
			case StoredType::ULongLong:
				return static_cast<float>(_primitiveData._ullong);
			case StoredType::Double:
				return static_cast<float>(_primitiveData._double);
			case StoredType::Float:
				return _primitiveData._float;
		}
		return 0;
	};
	operator double() const {
		switch (_currentType) {
			case StoredType::Empty:
			case StoredType::Void:
				return 0;
			case StoredType::Bool:
				return _primitiveData._bool;
			case StoredType::Int:
				return _primitiveData._int;
			case StoredType::UInt:
				return _primitiveData._uint;
			case StoredType::Double:
				return _primitiveData._double;
			case StoredType::LongLong:
				return static_cast<double>(_primitiveData._llong);
			case StoredType::ULongLong:
				return static_cast<double>(_primitiveData._ullong);
			case StoredType::Float:
				return static_cast<double>(_primitiveData._float);
		}
		return 0;
	};

	operator std::string() const {
		switch (_currentType) {
			case StoredType::Empty:
			case StoredType::Void:
				return "";
			case StoredType::Bool:
				return _primitiveData._bool ? "true" : "false";
			case StoredType::Int:
				return std::to_string(_primitiveData._int);
			case StoredType::UInt:
				return std::to_string(_primitiveData._uint);
			case StoredType::LongLong:
				return std::to_string(_primitiveData._llong);
			case StoredType::ULongLong:
				return std::to_string(_primitiveData._ullong);
			case StoredType::Float:
				return std::to_string(_primitiveData._float);
			case StoredType::Double:
				return std::to_string(_primitiveData._double);
			case StoredType::String:
				return *_primitiveData._stringPtr;
		}

	};

	// Enum conversions (Enum values in a variant are stored as an int)
	template <typename E> requires IsIntegerEnum<E>
	Variant(E data) noexcept {
		_primitiveData._int = static_cast<int>(data);
		_currentType = StoredType::Int;
	}

	template <typename E> requires IsIntegerEnum<E>
	operator E() const noexcept {
		return static_cast<E>(_primitiveData._int);
	}

	friend static bool operator==(const Variant& lhs, const Variant& rhs);
};