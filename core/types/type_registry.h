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
friend void engine_type_registry::type_registry::register_all_types(); \
static void _register_type(); \
static string _get_type() {return #NAME; } \
static string _get_parent_type() {return #DERIVED; } \

template <class Type>
concept IsDerivedFromObject = std::is_base_of<Object, Type>::value;

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
	class ClassMethodDefinition {
		public:
		ObjectRTTIModel::ObjectMethodDefinition methodMetadata;
		ClassMethodDefinition(ObjectRTTIModel::ObjectMethodDefinition methodInfo) {
			methodMetadata = methodInfo;
		}

		virtual Variant Call(Object* obj, std::vector<Variant> args) const = 0;
	};

	template <typename R, typename T, typename... Args> requires IsDerivedFromObject<T>
	class BindedClassMethodDefinition : public ClassMethodDefinition {
		private:
		bool _isConst;

		public:
		R(T::* constMethod)(Args...) const = nullptr;
		R(T::* method)(Args...) = nullptr;
		
		virtual Variant Call(Object* obj, std::vector<Variant> args) const override {
			bool success = false;
			Variant result;
			if (_isConst) {
				result = call_class_method_helper<R, T, Args...>(static_cast<T*>(obj), constMethod, args, methodMetadata.requiredArgCount, methodMetadata.defaultArgValues, success);
			}
			else {
				result = call_class_method_helper<R, T, Args...>(static_cast<T*>(obj), method, args, methodMetadata.requiredArgCount, methodMetadata.defaultArgValues, success);
			}
			return result;
		}

		BindedClassMethodDefinition(R(T::* met)(Args...) const, ObjectRTTIModel::ObjectMethodDefinition methodInfo): ClassMethodDefinition(methodInfo) {
			constMethod = met;
			_isConst = true;
		}
		BindedClassMethodDefinition(R(T::* met)(Args...), ObjectRTTIModel::ObjectMethodDefinition methodInfo) : ClassMethodDefinition(methodInfo) {
			method = met;
			_isConst = false;
		}
	};

	class EngineClass {
		friend class type_registry;
		private:
		string _className = "";

		map<string, ClassMethodDefinition*> _methods{};
		map<string, ObjectRTTIModel::ObjectPropertyDefinition> _properties{};
		public:
		string GetName() const { return _className; }
		bool HasMethod(string methodName) const { return _methods.contains(methodName); }
	};

	class type_registry {
		private:
		static map<string, EngineClass> _registered_classes;
		public:
		static void register_all_types();


		static void register_new_class(string new_class_name, string parent_class_name = "Object");

		template <typename R, typename T, typename... Args> requires IsDerivedFromObject<T>
		static void class_expose_method(string class_name, ObjectRTTIModel::ObjectMethodDefinition methodInfo, R(T::* func)(Args...)) {
			if (_registered_classes[class_name]._methods.contains(methodInfo.methodName)) {
				Log.Warn("TypeRegistry", "Attempted to redefine method " + class_name + "::" + methodInfo.methodName);
				return;
			}
			_registered_classes[class_name]._methods[methodInfo.methodName] = new BindedClassMethodDefinition<R, T, Args...>(func, methodInfo);
			return;
		};

		// Overload to expose member functions declared const
		template <typename R, typename T, typename... Args> requires IsDerivedFromObject<T>
		static void class_expose_method(string class_name, ObjectRTTIModel::ObjectMethodDefinition methodInfo, R(T::* func)(Args...) const) {
			if (_registered_classes[class_name]._methods.contains(methodInfo.methodName)) {
				Log.Warn("TypeRegistry", "Attempted to redefine method " + class_name + "::" + methodInfo.methodName);
				return;
			}
			_registered_classes[class_name]._methods[methodInfo.methodName] = new BindedClassMethodDefinition<R, T, Args...>(func, methodInfo);
			return;
		};

		static void class_define_property(string class_name, ObjectRTTIModel::ObjectPropertyDefinition def);


		static void log_all_type_data_debug() {
			for (const auto& [key, value] : _registered_classes) {
				std::cout << endl;
				std::cout << "Class: '" << value._className << endl;

				std::cout << "Methods:" << endl;
				for (const auto& [k, method] : value._methods) {
					std::cout << Variant::VariantTypeToString(method->methodMetadata.returnType) << " " << value._className << "::" << method->methodMetadata.methodName << "()" << endl;
					std::cout << "Required args:" << method->methodMetadata.requiredArgCount << endl;
				}

				std::cout << "Properties:" << endl;
				for (const auto& [k, prop] : value._properties) {
					std::cout << Variant::VariantTypeToString(prop.type) << " " << value._className << "::" << prop.propertyName << endl;
					std::cout << "getter, setter = " << prop.getterName << "()  " << prop.setterName << "()" << endl;
				}

			}
		}
	};
}