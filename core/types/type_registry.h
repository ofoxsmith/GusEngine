#pragma once
#include "core/globals.h"
#include <string>
#include <map>
#include <vector>
#include <functional>
#include <concepts>
#include <utility>
#include <type_traits>
#include <array>
#include <algorithm>
#include <iterator>
#include "object.h"
#include "variant_type.h"

using namespace std;

// This namespace contains macros, methods, and classes used to provide detailed runtime type information (RTTI) to the engine.
#define GUS_DECLARE_CLASS(NAME, DERIVED) \
friend class engine_type_registry::type_registry; \
static void _register_type(); \
public: \
static string _ClassNameStatic() {return #NAME;} \
static string _DerivedFromStatic() {return #DERIVED;} \
string _ClassName() override {return #NAME; } \
string _DerivedFrom() override {return #DERIVED; } \
private: \

template <class Type>
concept IsDerivedFromObject = std::is_base_of<Object, Type>::value;

template <typename T>
static Object* dynamic_constructor() {
	Object* ret = new T;
	return ret;
}

template <typename R, typename T, typename... Args, std::size_t... Is> requires IsDerivedFromObject<T>
static Variant call_class_method_helper_impl(T* obj, R(T::* method)(Args...), const Variant resolvedArgs[], std::index_sequence<Is...>) {
	if constexpr (std::is_void_v<R>) {		
		(obj->*method)(static_cast<Args>(resolvedArgs[Is])...);
		return Variant(Variant::Void);
	}
	else {
		return (obj->*method)(static_cast<Args>(resolvedArgs[Is])...);
	}
}

template <typename R, typename T, typename... Args, std::size_t... Is> requires IsDerivedFromObject<T>
static Variant call_class_method_helper_impl(T* obj, R(T::* method)(Args...)const, const Variant resolvedArgs[], std::index_sequence<Is...>) {
	if constexpr (std::is_void_v<R>) {
		(obj->*method)(static_cast<Args>(resolvedArgs[Is])...);
		return Variant(Variant::Void);
	}
	else {
		return (obj->*method)(static_cast<Args>(resolvedArgs[Is])...);
	}
}

template <typename R, typename T, typename... Args> requires IsDerivedFromObject<T>
static Variant call_class_method_helper(T* obj, R(T::* method)(Args...), const vector<Variant> args, int requiredArgCount, const vector<Variant>& defaultArgs, bool& callSuccessfull) {
	if (args.size() < requiredArgCount) {
		callSuccessfull = false;
		return Variant(Variant::Void);
	}

	Variant resolvedArgs[sizeof...(Args) == 0 ? 1 : sizeof...(Args)]{};
	for (int i = 0; i < (int)sizeof...(Args); i++) {
		if (args.size() < i) {
			resolvedArgs[i] = args[i];
		}
		else {
			resolvedArgs[i] = defaultArgs[i];
		}
	}

	return call_class_method_helper_impl(obj, method, resolvedArgs, std::index_sequence_for<Args...>());
}

template <typename R, typename T, typename... Args> requires IsDerivedFromObject<T>
static Variant call_class_method_helper(T* obj, R(T::* method)(Args...) const, const vector<Variant> args, int requiredArgCount, const vector<Variant>& defaultArgs, bool& callSuccessfull) {
	if (args.size() < requiredArgCount) {
		callSuccessfull = false;
		return Variant(Variant::Void);
	}

	Variant resolvedArgs[sizeof...(Args) == 0 ? 1 : sizeof...(Args)]{};
	for (int i = 0; i < (int)sizeof...(Args); i++) {
		if (args.size() < i) {
			resolvedArgs[i] = args[i];
		}
		else {
			resolvedArgs[i] = defaultArgs[i];
		}
	}

	return call_class_method_helper_impl(obj, method, resolvedArgs, std::index_sequence_for<Args...>());
}

namespace engine_type_registry {
	class ObjectMethod {
		public:
		ObjectRTTIModel::ObjectMethodDefinition methodMetadata;
		ObjectMethod(ObjectRTTIModel::ObjectMethodDefinition methodInfo) {
			methodMetadata = methodInfo;
		}

		virtual Variant Call(Object* obj, std::vector<Variant> args) const = 0;
	};

	template <typename R, typename T, typename... Args> requires IsDerivedFromObject<T>
	class BindedObjectMethod : public ObjectMethod {
		private:
		bool _isConst;

		public:
		R(T::* constMethod)(Args...) const = nullptr;
		R(T::* method)(Args...) = nullptr;
		
		virtual Variant Call(Object* obj, std::vector<Variant> args) const override {
			bool success = false;
			if (_isConst) {
				return call_class_method_helper<R, T, Args...>(static_cast<T*>(obj), constMethod, args, methodMetadata.requiredArgCount, methodMetadata.defaultArgValues, success);
			}
			else {
				return call_class_method_helper<R, T, Args...>(static_cast<T*>(obj), method, args, methodMetadata.requiredArgCount, methodMetadata.defaultArgValues, success);
			}
		}
		
		BindedObjectMethod(R(T::* met)(Args...) const, ObjectRTTIModel::ObjectMethodDefinition methodInfo): ObjectMethod(methodInfo) {
			constMethod = met;
			_isConst = true;
		}
		BindedObjectMethod(R(T::* met)(Args...), ObjectRTTIModel::ObjectMethodDefinition methodInfo) : ObjectMethod(methodInfo) {
			method = met;
			_isConst = false;
		}
	};

	class EngineClass {
		friend class type_registry;
		friend class Object;
		private:
		string _className = "";
		string _parentClassName = "";
		EngineClass* _inherits = nullptr;
		map<string, ObjectRTTIModel::ObjectMethodDefinition> _methods{};
		map<string, ObjectMethod*> _methodBinds{};
		map<string, ObjectRTTIModel::ObjectPropertyDefinition> _properties{};
		public:
		string GetName() const { return _className; }
		bool HasMethod(string methodName) const { return _methods.contains(methodName); }
		Object* (*_dynamic_constructor)() = nullptr;
	};

	class type_registry {
		private:
		public:
		static void register_all_types();
		static void register_new_class(string new_class_name, string parent_class_name = "Object");
		static map<string, EngineClass> _registered_classes;

		template <typename T>
		static void register_class() {
			T::_register_type();
			EngineClass cls = _registered_classes[T::_ClassNameStatic()];
			cls._dynamic_constructor = &(dynamic_constructor<T>);

		};

		template <typename R, typename T, typename... Args> requires IsDerivedFromObject<T>
		static void class_expose_method(string class_name, ObjectRTTIModel::ObjectMethodDefinition methodInfo, R(T::* func)(Args...)) {
			if (_registered_classes[class_name]._methods.contains(methodInfo.methodName)) {
				Log.Warn("TypeRegistry", "Attempted to redefine method " + class_name + "::" + methodInfo.methodName);
				return;
			}
			_registered_classes[class_name]._methods[methodInfo.methodName] = methodInfo;
			_registered_classes[class_name]._methodBinds[methodInfo.methodName] = new BindedObjectMethod<R, T, Args...>(func, methodInfo);
			return;
		};

		// Overload to expose member functions declared const
		template <typename R, typename T, typename... Args> requires IsDerivedFromObject<T>
		static void class_expose_method(string class_name, ObjectRTTIModel::ObjectMethodDefinition methodInfo, R(T::* func)(Args...) const) {
			if (_registered_classes[class_name]._methods.contains(methodInfo.methodName)) {
				Log.Warn("TypeRegistry", "Attempted to redefine method " + class_name + "::" + methodInfo.methodName);
				return;
			}
			_registered_classes[class_name]._methods[methodInfo.methodName] = methodInfo;
			_registered_classes[class_name]._methodBinds[methodInfo.methodName] = new BindedObjectMethod<R, T, Args...>(func, methodInfo);
			return;
		};

		static void class_define_property(string class_name, ObjectRTTIModel::ObjectPropertyDefinition def);

	};
}