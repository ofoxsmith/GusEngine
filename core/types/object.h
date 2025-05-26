#pragma once
#include "variant_type.h"
#include <vector>
#include <map>
// Base class for everything, provides RTTI and engine boilerplate.

namespace ObjectRTTIModel {

	struct ObjectPropertyDefinition {

		enum PropertyFlags : int {
			NONE = 0,
			// The property is readonly.
			READ_ONLY = 1 << 0,
			// The property won't be saved when serialising a resource.
			NO_SAVE = 1 << 1,
		};

		string propertyName = "";
		int flags = 0;
		Variant::StoredType type = Variant::StoredType::Void;
		string getterName = "";
		string setterName = "";
		ObjectPropertyDefinition() {}
		ObjectPropertyDefinition(string propName, Variant::StoredType ty, PropertyFlags pflags, string getter, string setter = "") {
			propertyName = propName;
			type = ty;
			flags = pflags;
			getterName = getter;
			setterName = setter;
		}
	};

	struct ObjectMethodDefinition {
		string methodName = "";
		Variant::StoredType returnType = Variant::StoredType::Void;
		int requiredArgCount = 0;
		vector<Variant> defaultArgValues{};
		ObjectMethodDefinition() {}
		ObjectMethodDefinition(string name, Variant::StoredType rType, int reqArgCount = 0, vector<Variant> defaultArgs = {}) {
			methodName = name;
			returnType = rType;
			requiredArgCount = reqArgCount;
			defaultArgValues = defaultArgs;
		}
	};
}

class Object {
	private:
	Variant _callInternal(string methodName, vector<Variant> args);
	protected:

	public:
	virtual string _ClassName() { return "Object"; };
	virtual string _DerivedFrom() { return ""; };
	bool _IsDerivedFrom(string className);

	map<string, ObjectRTTIModel::ObjectMethodDefinition> _GetMethodList();
	bool _HasMethod(string methodName);
	template <typename... Args>
	Variant _Call(string methodName, const Args... args) {
		std::vector<Variant> argVector = { args... };
		return _callInternal(methodName, argVector);
	};

	map<string, ObjectRTTIModel::ObjectPropertyDefinition> _GetPropertyList();
	bool _HasProperty(string propertyName);
	void _Set(string property, Variant value);
	Variant _Get(string property);

	virtual void _Init() {}
	virtual ~Object() {}

};