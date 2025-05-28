#pragma once
#include <string>
#include "core/globals.h"
#include <concepts>
#include <type_traits>

template <typename T>
concept IsIntegerEnum = (std::is_enum<T>::value) && (std::is_same_v<std::underlying_type_t<T>, int32_t>);


// Implementation of a variant data type, which can dynamically hold primitve data types, used to correctly cast data for use with RTTI.
struct Variant {
	enum StoredType: short {
		// NOTE: Empty and Void represent two different types - A variant that has not been assigned, vs an assigned variant holding nothing.
		Empty = 0,
		Void = (1u << 0),

		// Primitive types:
		Bool = (1u << 1),
		Int32 = (1u << 2),
		UInt32 = (1u << 3),
		Int64 = (1u << 4),
		UInt64 = (1u << 5),
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
			case StoredType::Int32:
				return "Int32";
			case StoredType::UInt32:
				return "UInt32";
			case StoredType::Int64:
				return "Int64";
			case StoredType::UInt64:
				return "UInt64";
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
		int32_t _int;
		uint32_t _uint;
		int64_t _llong;
		uint64_t _ullong;
		float _float;
		double _double;

		std::string* _stringPtr;
	} _primitiveData;

	StoredType _currentType;

	static bool _same(const Variant& v1, const Variant& v2);

	public:
	StoredType Type() const { return _currentType; }

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
		return (_currentType & (StoredType::Int32 | StoredType::UInt32 | StoredType::Int64 | StoredType::UInt64));
	};

	bool IsNumeric() inline const {
		return (_currentType & (StoredType::Int32 | StoredType::UInt32 | StoredType::Int64 | StoredType::UInt64 | StoredType::Float | StoredType::Double));
	};

	bool IsSignedInt() inline const {
		return (_currentType & (StoredType::Int32 | StoredType::Int64));
	};

	void CastTo(StoredType newType);
	static char* BinarySerialise(Variant v);
	static int32_t BinarySerialisationLength(char* bin);
	static std::string StringSerialise(Variant v);
	static Variant FromString(std::string* str);
	
	//// Operators and constructors

	Variant() { 
		_primitiveData._int = 0; 
		_currentType = StoredType::Empty; 
	};
	Variant(StoredType type) {
		_primitiveData._int = 0;
		_currentType = type;
	}
	
	Variant(bool data);
	Variant(int32_t data);
	Variant(uint32_t data);
	Variant(int64_t data);
	Variant(uint64_t data);
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
			case StoredType::Int32:
				return _primitiveData._int;
			case StoredType::UInt32:
				return _primitiveData._uint;
			case StoredType::Int64:
				return _primitiveData._llong;
			case StoredType::UInt64:
				return _primitiveData._ullong;
			case StoredType::Float:
				return _primitiveData._float;
			case StoredType::Double:
				return _primitiveData._double;
		}
		return false;
	};
	operator int32_t() const {
		switch (_currentType) {
			case StoredType::Empty:
			case StoredType::Void:
				return 0;
			case StoredType::Int64:
				return static_cast<int32_t>(_primitiveData._llong);
			case StoredType::UInt64:
				return static_cast<int32_t>(_primitiveData._ullong);
			case StoredType::Float:
				return static_cast<int32_t>(_primitiveData._float);
			case StoredType::Double:
				return static_cast<int32_t>(_primitiveData._double);
			case StoredType::Bool:
				return _primitiveData._bool;
			case StoredType::Int32:
				return _primitiveData._int;
			case StoredType::UInt32:
				return _primitiveData._uint;
		}
		return 0;
	};
	operator uint32_t() const {
		switch (_currentType) {
			case StoredType::Empty:
			case StoredType::Void:
				return 0;
			case StoredType::Bool:
				return _primitiveData._bool;
			case StoredType::Int32:
				return _primitiveData._int;
			case StoredType::UInt32:
				return _primitiveData._uint;
			case StoredType::Int64:
				return static_cast<uint32_t>(_primitiveData._llong);
			case StoredType::UInt64:
				return static_cast<uint32_t>(_primitiveData._ullong);
			case StoredType::Float:
				return static_cast<uint32_t>(_primitiveData._float);
			case StoredType::Double:
				return static_cast<uint32_t>(_primitiveData._double);
		}
		return 0;
	};

	operator int64_t() const {
		switch (_currentType) {
			case StoredType::Empty:
			case StoredType::Void:
				return 0;
			case StoredType::Bool:
				return _primitiveData._bool;
			case StoredType::Int32:
				return _primitiveData._int;
			case StoredType::UInt32:
				return _primitiveData._uint;
			case StoredType::Int64:
				return _primitiveData._llong;
			case StoredType::UInt64:
				return _primitiveData._ullong;
			case StoredType::Float:
				return static_cast<int64_t>(_primitiveData._float);
			case StoredType::Double:
				return static_cast<int64_t>(_primitiveData._double);
		}
		return 0;

	};
	operator uint64_t() const {
		switch (_currentType) {
			case StoredType::Empty:
			case StoredType::Void:
				return 0;
			case StoredType::Bool:
				return _primitiveData._bool;
			case StoredType::Int32:
				return _primitiveData._int;
			case StoredType::UInt32:
				return _primitiveData._uint;
			case StoredType::Int64:
				return _primitiveData._llong;
			case StoredType::UInt64:
				return _primitiveData._ullong;
			case StoredType::Float:
				return static_cast<uint64_t>(_primitiveData._float);
			case StoredType::Double:
				return static_cast<uint64_t>(_primitiveData._double);
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
			case StoredType::Int32:
				return static_cast<float>(_primitiveData._int);
			case StoredType::UInt32:
				return static_cast<float>(_primitiveData._uint);
			case StoredType::Int64:
				return static_cast<float>(_primitiveData._llong);
			case StoredType::UInt64:
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
			case StoredType::Int32:
				return _primitiveData._int;
			case StoredType::UInt32:
				return _primitiveData._uint;
			case StoredType::Double:
				return _primitiveData._double;
			case StoredType::Int64:
				return static_cast<double>(_primitiveData._llong);
			case StoredType::UInt64:
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
			case StoredType::Int32:
				return std::to_string(_primitiveData._int);
			case StoredType::UInt32:
				return std::to_string(_primitiveData._uint);
			case StoredType::Int64:
				return std::to_string(_primitiveData._llong);
			case StoredType::UInt64:
				return std::to_string(_primitiveData._ullong);
			case StoredType::Float:
				return std::to_string(_primitiveData._float);
			case StoredType::Double:
				return std::to_string(_primitiveData._double);
			case StoredType::String:
				return *_primitiveData._stringPtr;
		}

	};

	// Enum conversions (Enum values in a variant are stored as an int32_t)
	template <typename E> requires IsIntegerEnum<E>
	Variant(E data) noexcept {
		_primitiveData._int = static_cast<int32_t>(data);
		_currentType = StoredType::Int32;
	}

	template <typename E> requires IsIntegerEnum<E>
	operator E() const noexcept {
		return static_cast<E>(_primitiveData._int);
	}

	friend static bool operator==(const Variant& lhs, const Variant& rhs);
};