#pragma once
#include <string>

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
	// All data is stored in a single union type. 
	// Primitives are stored directly in the union, while complex types have an access pointer stored in the union.
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

	
#pragma region Casting operators
	// Each supported type has a weakCast operator, which could result in an unexpected value/data loss.
	template <typename T>
	static T _weakCast(const variantData& data, StoredType currentType) {
		static_assert(sizeof(T) == 0, "Only specializations of _weakCast can be used");
	};

	// Each supported type has a strictCast operator, which is guaranteed to safely cast between types without data loss.
	template <typename T>
	static T _strictCast(const variantData& data, StoredType currentType) {
		static_assert(sizeof(T) == 0, "Only specializations of _strictCast can be used");
	};

	template <> static bool _weakCast<bool>(const variantData& data, StoredType currentType) {
		switch (currentType) {
			case String:
				return *data._stringPtr == "true";
		}
		return _strictCast<bool>(data, currentType);
	}

	template<>
	static bool _strictCast<bool>(const variantData& data, StoredType currentType) {
		switch (currentType) {
			case Empty:
			case Void:
				return false;
			case Bool:
				return data._bool;
			case Int:
				return data._int;
			case UInt:
				return data._uint;
			case LongLong:
				return data._llong;
			case ULongLong:
				return data._ullong;
			case Float:
				return data._float;
			case Double:
				return data._double;
		}
		return false;

	}

	template <> static int _weakCast<int>(const variantData& data, StoredType currentType) {
		switch (currentType) {
			case LongLong:
				return static_cast<int>(data._llong);
			case ULongLong:
				return static_cast<int>(data._ullong);
			case Float:
				return static_cast<int>(data._float);
			case Double:
				return static_cast<int>(data._double);
			case String:
				return std::stoi(*data._stringPtr);
		}
		return _strictCast<int>(data, currentType);
	}

	template<>
	static int _strictCast<int>(const variantData& data, StoredType currentType) {
		switch (currentType) {
			case Empty:
			case Void:
				return 0;
			case Bool:
				return data._bool;
			case Int:
				return data._int;
			case UInt:
				return data._uint;
		}
		return 0;
	}


	template <> static unsigned int _weakCast<unsigned int>(const variantData& data, StoredType currentType) {
		switch (currentType) {
			case LongLong:
				return static_cast<unsigned int>(data._llong);
			case ULongLong:
				return static_cast<unsigned int>(data._ullong);
			case Float:
				return static_cast<unsigned int>(data._float);
			case Double:
				return static_cast<unsigned int>(data._double);
			case String:
				return std::stoul(*data._stringPtr);
		}
		return _strictCast<unsigned int>(data, currentType);
	}

	template<>
	static unsigned int _strictCast<unsigned int>(const variantData& data, StoredType currentType) {
		switch (currentType) {
			case Empty:
			case Void:
				return 0;
			case Bool:
				return data._bool;
			case Int:
				return data._int;
			case UInt:
				return data._uint;
		}
		return 0;
	}

	template <> static long long _weakCast<long long>(const variantData& data, StoredType currentType) {
		switch (currentType) {
			case Float:
				return static_cast<long long>(data._float);
			case Double:
				return static_cast<long long>(data._double);
			case String:
				return std::stoll(*data._stringPtr);
		}
		return _strictCast<long long>(data, currentType);
	}

	template<>
	static long long _strictCast<long long>(const variantData& data, StoredType currentType) {
		switch (currentType) {
			case Empty:
			case Void:
				return 0;
			case Bool:
				return data._bool;
			case Int:
				return data._int;
			case UInt:
				return data._uint;
			case LongLong:
				return data._llong;
			case ULongLong:
				return data._ullong;
		}
		return 0;
	}

	template <> static unsigned long long _weakCast<unsigned long long>(const variantData& data, StoredType currentType) {
		switch (currentType) {
			case Float:
				return static_cast<unsigned long long>(data._float);
			case Double:
				return static_cast<unsigned long long>(data._double);
			case String:
				return std::stoull(*data._stringPtr);
		}
		return _strictCast<unsigned long long>(data, currentType);
	}

	template<>
	static unsigned long long _strictCast<unsigned long long>(const variantData& data, StoredType currentType) {
		switch (currentType) {
			case Empty:
			case Void:
				return 0;
			case Bool:
				return data._bool;
			case Int:
				return data._int;
			case UInt:
				return data._uint;
			case LongLong:
				return data._llong;
			case ULongLong:
				return data._ullong;
		}
		return 0;

	}

	template <> static float _weakCast<float>(const variantData& data, StoredType currentType) {
		switch (currentType) {
			case Int:
				return static_cast<float>(data._int);
			case UInt:
				return static_cast<float>(data._uint);
			case LongLong:
				return static_cast<float>(data._llong);
			case ULongLong:
				return static_cast<float>(data._ullong);
			case Double:
				return static_cast<float>(data._double);
			case String:
				return std::stof(*data._stringPtr);
		}
		return _strictCast<float>(data, currentType);
	}

	template<>
	static float _strictCast<float>(const variantData& data, StoredType currentType) {
		switch (currentType) {
			case Empty:
			case Void:
				return 0;
			case Bool:
				return data._bool;
			case Float:
				return data._float;
		}
		return 0;

	}

	template <> static double _weakCast<double>(const variantData& data, StoredType currentType) {
		switch (currentType) {
			case LongLong:
				return static_cast<double>(data._llong);
			case ULongLong:
				return static_cast<double>(data._ullong);
			case Float:
				return static_cast<double>(data._float);
			case String:
				return std::stod(*data._stringPtr);
		}
		return _strictCast<double>(data, currentType);
	}

	template<>
	static double _strictCast<double>(const variantData& data, StoredType currentType) {
		switch (currentType) {
			case Empty:
			case Void:
				return 0;
			case Bool:
				return data._bool;
			case Int:
				return data._int;
			case UInt:
				return data._uint;
			case Double:
				return data._double;
		}
		return 0;
	}

	template <> static std::string _weakCast<std::string>(const variantData& data, StoredType currentType) {
		switch (currentType) {
			case Int:
				return std::to_string(data._int);
			case UInt:
				return std::to_string(data._uint);
			case LongLong:
				return std::to_string(data._llong);
			case ULongLong:
				return std::to_string(data._ullong);
			case Float:
				return std::to_string(data._float);
			case Double:
				return std::to_string(data._double);
		}
		return _strictCast<std::string>(data, currentType);
	}

	template<>
	static std::string _strictCast<std::string>(const variantData& data, StoredType currentType) {
		switch (currentType) {
			case Empty:
			case Void:
				return "";
			case Bool:
				return data._bool ? "true" : "false";
			case String:
				return *data._stringPtr;
		}
		return "";

	}
#pragma endregion

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
			case Empty:
			case Void:
				return false;
			case Bool:
				return _primitiveData._bool;
			case Int:
				return _primitiveData._int == 0;
			case UInt:
				return _primitiveData._uint == 0;
			case LongLong:
				return _primitiveData._llong == 0;
			case ULongLong:
				return _primitiveData._ullong == 0;
			case Float:
				return _primitiveData._float == 0;
			case Double:
				return _primitiveData._double == 0;
			case String:
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

	void CastTo(StoredType newType);
	void CastToStrict(StoredType newType);
	std::string Stringify();
	static Variant FromString(std::string str);

	//// Operators and constructors

	Variant() { 
		_primitiveData._int = 0; 
		_currentType = StoredType::Empty; 
	};
	Variant(bool data);
	Variant(int data);
	Variant(unsigned int data);
	Variant(long long data);
	Variant(unsigned long long data);
	Variant(float data);
	Variant(double data);
	Variant(std::string data);

	Variant(const Variant& other);
	Variant(Variant&& other) noexcept;
	~Variant() {
		Clear();
	}

	Variant& operator=(Variant other) {
		_swap(*this, other);
		return *this;
	}

	operator bool() const {
		return _weakCast<bool>(_primitiveData, _currentType);
	};
	operator int() const {
		return _weakCast<int>(_primitiveData, _currentType);
	};
	operator unsigned int() const {
		return _weakCast<unsigned int>(_primitiveData, _currentType);
	};
	operator long long() const {
		return _weakCast<long long>(_primitiveData, _currentType);
	};
	operator unsigned long long() const {
		return _weakCast<unsigned long long>(_primitiveData, _currentType);
	};
	operator float() const {
		return _weakCast<float>(_primitiveData, _currentType);
	};
	operator double() const {
		return _weakCast<double>(_primitiveData, _currentType);
	};
	operator std::string() const {
		return _weakCast<std::string>(_primitiveData, _currentType);
	};

	friend static bool operator==(const Variant& lhs, const Variant& rhs);
};
