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
namespace engine_type_registry {
	typedef string data_type;
	typedef unsigned int class_id;
}

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
	struct ClassPropertyDefinition {
		string propertyName = "";
		Variant::StoredType type = Variant::StoredType::Void;
		bool isReadOnly = false;
		string getterName = "";
		string setterName = "";
		ClassPropertyDefinition() {}
		ClassPropertyDefinition(string propName, Variant::StoredType ty, bool readOnly, string getter, string setter = "") {
			propertyName = propName;
			type = ty;
			isReadOnly = readOnly;
			getterName = getter;
			setterName = setter;
		}
	};

	struct ClassMethodDescription {
		string methodName = "";
		Variant::StoredType returnType = Variant::StoredType::Void;
		int requiredArgCount = 0;
		vector<Variant> defaultArgValues{};
		ClassMethodDescription() {}
		ClassMethodDescription(string name, Variant::StoredType rType, int reqArgCount = 0, vector<Variant> defaultArgs = {}) {
			methodName = name;
			returnType = rType;
			requiredArgCount = reqArgCount;
			defaultArgValues = defaultArgs;
		}
	};

	class ClassMethodDefinition {
		public:
		ClassMethodDescription methodMetadata;
		ClassMethodDefinition(ClassMethodDescription methodInfo) {
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

		BindedClassMethodDefinition(R(T::* met)(Args...) const, ClassMethodDescription methodInfo): ClassMethodDefinition(methodInfo) {
			constMethod = met;
			_isConst = true;
		}
		BindedClassMethodDefinition(R(T::* met)(Args...), ClassMethodDescription methodInfo) : ClassMethodDefinition(methodInfo) {
			method = met;
			_isConst = false;
		}
	};

	class EngineClass {
		friend class type_registry;
		private:
		string _className = "";
		class_id _classId = -1;
		map<string, ClassMethodDefinition*> _methods{};
		map<string, ClassPropertyDefinition> _properties{};
		public:
		string GetName() const { return _className; }
		class_id GetId() const { return _classId; }
		bool HasMethod(string methodName) const { return _methods.contains(methodName); }
	};

	class type_registry {
		private:
		static class_id _currentId;
		static map<class_id, EngineClass> _registered_classes;
		static map<string, class_id> _registered_classes_names;
		public:
		static void register_all_types();


		static class_id register_new_class(string new_class_name, string parent_class_name = "Object");
		static class_id get_registered_class_name(string c_name);

		template <typename EnumType>
		static void class_register_enum(class_id c_id, string enum_name) {
			
		};

		template <typename R, typename T, typename... Args> requires IsDerivedFromObject<T>
		static void class_expose_method(class_id c_id, ClassMethodDescription methodInfo, R(T::* func)(Args...)) {
			if (_registered_classes[c_id]._methods.contains(methodInfo.methodName)) {
				Log.Warn("TypeRegistry", "Attempted to redefine method " + _registered_classes[c_id]._className + "::" + methodInfo.methodName);
				return;
			}
			_registered_classes[c_id]._methods[methodInfo.methodName] = new BindedClassMethodDefinition<R, T, Args...>(func, methodInfo);
		};

		// Overload to expose member functions declared const
		template <typename R, typename T, typename... Args> requires IsDerivedFromObject<T>
		static void class_expose_method(class_id c_id, ClassMethodDescription methodInfo, R(T::* func)(Args...) const) {
			if (_registered_classes[c_id]._methods.contains(methodInfo.methodName)) {
				Log.Warn("TypeRegistry", "Attempted to redefine method " + _registered_classes[c_id]._className + "::" + methodInfo.methodName);
				return;
			}
			_registered_classes[c_id]._methods[methodInfo.methodName] = new BindedClassMethodDefinition<R, T, Args...>(func, methodInfo);
		};

		static void class_define_property(class_id c_id, ClassPropertyDefinition def);


		static void log_all_type_data_debug() {
			for (const auto& [key, value] : _registered_classes) {
				std::cout << endl;
				std::cout << "Class: '" << value._className << "' " << value._classId << endl;

				for (const auto& [k, method] : value._methods) {
					std::cout << enum_get_underlying(method->methodMetadata.returnType) << " " << value._className << "::" << method->methodMetadata.methodName << "()" << endl;
					std::cout << "Required args:" << method->methodMetadata.requiredArgCount << endl;
				}

				for (const auto& [k, prop] : value._properties) {
					std::cout << enum_get_underlying(prop.type) << " " << value._className << "::" << prop.propertyName << endl;
					std::cout << "getter, setter = " << prop.getterName << "()  " << prop.setterName << "()" << endl;
				}

			}
		}
	};
}