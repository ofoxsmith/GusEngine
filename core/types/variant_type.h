#pragma once
#include <string>
#include "core/globals.h"
#include <concepts>
#include <type_traits>

template <typename T>
concept IsIntegerEnum = (std::is_enum<T>::value) && (std::is_same_v<std::underlying_type_t<T>, int>);


// Implementation of a variant data type, which can dynamically hold primitve data types, used to correctly cast data for use with RTTI.
struct Variant {
	enum StoredType {
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

	// Each supported type has an _cast method, converts it to other Variant data types.
	// If strictCast is false, weak casts which could result in an unexpected value/data loss are used.
	// If strictCast is true, stricter casting rules are used, which guarantee to safely cast between types without data loss.
	template <typename T>
	static T _cast(const variantData& data, StoredType currentType, bool strictCast = false) {
		static_assert(sizeof(T) == 0, "Only specializations of _cast can be used");
	};

	template <> static bool _cast<bool>(const variantData& data, StoredType currentType, bool strictCast) {
		if (!strictCast) {
			switch (currentType) {
				case StoredType::String:
					return *data._stringPtr == "true";
			}
		}

		switch (currentType) {
			case StoredType::Empty:
			case StoredType::Void:
				return false;
			case StoredType::Bool:
				return data._bool;
			case StoredType::Int:
				return data._int;
			case StoredType::UInt:
				return data._uint;
			case StoredType::LongLong:
				return data._llong;
			case StoredType::ULongLong:
				return data._ullong;
			case StoredType::Float:
				return data._float;
			case StoredType::Double:
				return data._double;
		}
		return false;
	}

	template <> 
	static int _cast<int>(const variantData& data, StoredType currentType, bool strictCast) {
		if (!strictCast) {
			switch (currentType) {
				case StoredType::LongLong:
					return static_cast<int>(data._llong);
				case StoredType::ULongLong:
					return static_cast<int>(data._ullong);
				case StoredType::Float:
					return static_cast<int>(data._float);
				case StoredType::Double:
					return static_cast<int>(data._double);
				case StoredType::String:
					return std::stoi(*data._stringPtr);
			}
		}

		switch (currentType) {
			case StoredType::Empty:
			case StoredType::Void:
				return 0;
			case StoredType::Bool:
				return data._bool;
			case StoredType::Int:
				return data._int;
			case StoredType::UInt:
				return data._uint;
		}
		return 0;

	}

	template <> 
	static unsigned int _cast<unsigned int>(const variantData& data, StoredType currentType, bool strictCast) {
		if (!strictCast) {
			switch (currentType) {
				case StoredType::LongLong:
					return static_cast<unsigned int>(data._llong);
				case StoredType::ULongLong:
					return static_cast<unsigned int>(data._ullong);
				case StoredType::Float:
					return static_cast<unsigned int>(data._float);
				case StoredType::Double:
					return static_cast<unsigned int>(data._double);
				case StoredType::String:
					return std::stoul(*data._stringPtr);
			}
		}

		switch (currentType) {
			case StoredType::Empty:
			case StoredType::Void:
				return 0;
			case StoredType::Bool:
				return data._bool;
			case StoredType::Int:
				return data._int;
			case StoredType::UInt:
				return data._uint;
		}
		return 0;
	}

	template <> static long long _cast<long long>(const variantData& data, StoredType currentType, bool strictCast) {
		if (!strictCast) {
			switch (currentType) {
				case StoredType::Float:
					return static_cast<long long>(data._float);
				case StoredType::Double:
					return static_cast<long long>(data._double);
				case StoredType::String:
					return std::stoll(*data._stringPtr);
			}
		}
		switch (currentType) {
			case StoredType::Empty:
			case StoredType::Void:
				return 0;
			case StoredType::Bool:
				return data._bool;
			case StoredType::Int:
				return data._int;
			case StoredType::UInt:
				return data._uint;
			case StoredType::LongLong:
				return data._llong;
			case StoredType::ULongLong:
				return data._ullong;
		}
		return 0;
	}

	template <> 
	static unsigned long long _cast<unsigned long long>(const variantData& data, StoredType currentType, bool strictCast) {
		if (!strictCast) {
			switch (currentType) {
				case StoredType::Float:
					return static_cast<unsigned long long>(data._float);
				case StoredType::Double:
					return static_cast<unsigned long long>(data._double);
				case StoredType::String:
					return std::stoull(*data._stringPtr);
			}
		}

		switch (currentType) {
			case StoredType::Empty:
			case StoredType::Void:
				return 0;
			case StoredType::Bool:
				return data._bool;
			case StoredType::Int:
				return data._int;
			case StoredType::UInt:
				return data._uint;
			case StoredType::LongLong:
				return data._llong;
			case StoredType::ULongLong:
				return data._ullong;
		}
		return 0;

	}

	template <> static float _cast<float>(const variantData& data, StoredType currentType, bool strictCast) {
		if (!strictCast) {
			switch (currentType) {
				case StoredType::Int:
					return static_cast<float>(data._int);
				case StoredType::UInt:
					return static_cast<float>(data._uint);
				case StoredType::LongLong:
					return static_cast<float>(data._llong);
				case StoredType::ULongLong:
					return static_cast<float>(data._ullong);
				case StoredType::Double:
					return static_cast<float>(data._double);
				case StoredType::String:
					return std::stof(*data._stringPtr);
			}
		}
		switch (currentType) {
			case StoredType::Empty:
			case StoredType::Void:
				return 0;
			case StoredType::Bool:
				return data._bool;
			case StoredType::Float:
				return data._float;
		}
		return 0;
	}

	template <> static double _cast<double>(const variantData& data, StoredType currentType, bool strictCast) {
		if (!strictCast) {
			switch (currentType) {
				case StoredType::LongLong:
					return static_cast<double>(data._llong);
				case StoredType::ULongLong:
					return static_cast<double>(data._ullong);
				case StoredType::Float:
					return static_cast<double>(data._float);
				case StoredType::String:
					return std::stod(*data._stringPtr);
			}
		}
		switch (currentType) {
			case StoredType::Empty:
			case StoredType::Void:
				return 0;
			case StoredType::Bool:
				return data._bool;
			case StoredType::Int:
				return data._int;
			case StoredType::UInt:
				return data._uint;
			case StoredType::Double:
				return data._double;
		}
		return 0;

	}

	template <> static std::string _cast<std::string>(const variantData& data, StoredType currentType, bool strictCast) {
		if (!strictCast) {
			switch (currentType) {
				case StoredType::Int:
					return std::to_string(data._int);
				case StoredType::UInt:
					return std::to_string(data._uint);
				case StoredType::LongLong:
					return std::to_string(data._llong);
				case StoredType::ULongLong:
					return std::to_string(data._ullong);
				case StoredType::Float:
					return std::to_string(data._float);
				case StoredType::Double:
					return std::to_string(data._double);
			}
		}
		switch (currentType) {
			case StoredType::Empty:
			case StoredType::Void:
				return "";
			case StoredType::Bool:
				return data._bool ? "true" : "false";
			case StoredType::String:
				return *data._stringPtr;
		}
		return "";
	}

	public:
	StoredType Type() const { return _currentType; }

	void Clear() {
		if (_currentType & StoredType::String) {
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
			case StoredType::Empty:
			case StoredType::Void:
				return false;
			case StoredType::Bool:
				return _primitiveData._bool;
			case StoredType::Int:
				return _primitiveData._int == 0;
			case StoredType::UInt:
				return _primitiveData._uint == 0;
			case StoredType::LongLong:
				return _primitiveData._llong == 0;
			case StoredType::ULongLong:
				return _primitiveData._ullong == 0;
			case StoredType::Float:
				return _primitiveData._float == 0;
			case StoredType::Double:
				return _primitiveData._double == 0;
			case StoredType::String:
				return (_primitiveData._stringPtr)->empty();
		}
		return false;
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

	void CastTo(StoredType newType, bool castStrictly = false);
	std::string Stringify();
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
		return _cast<bool>(_primitiveData, _currentType);
	};
	operator int() const {
		return _cast<int>(_primitiveData, _currentType);
	};
	operator unsigned int() const {
		return _cast<unsigned int>(_primitiveData, _currentType);
	};
	operator long long() const {
		return _cast<long long>(_primitiveData, _currentType);
	};
	operator unsigned long long() const {
		return _cast<unsigned long long>(_primitiveData, _currentType);
	};
	operator float() const {
		return _cast<float>(_primitiveData, _currentType);
	};
	operator double() const {
		return _cast<double>(_primitiveData, _currentType);
	};
	operator std::string() const {
		return _cast<std::string>(_primitiveData, _currentType);
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