#pragma once
#include <string>
#include <map>
#include <vector>
#include <functional>
#include <concepts>
#include <utility>
#include <type_traits>
#include <array>
#include "object.h"
#include "variant_type.h"

using namespace std;

// This namespace contains macros, methods, and classes used to provide detailed runtime type information (RTTI) to the engine.
namespace engine_type_registry {
	typedef string data_type;
	typedef unsigned int class_id;
}

#define GUS_CLASS_DERIVED(NAME, DERIVED) \
friend void engine_type_registry::type_registry::register_all_types(); \
static void _register_resource(engine_type_registry::class_id cId); \
static string _get_type() {return #NAME; } \
static string _get_parent_type() {return #DERIVED; } \
static void _register_type() { \
engine_type_registry::class_id cId = engine_type_registry::type_registry::register_new_resource(#NAME, #DERIVED);\
_register_resource(cId);\
} \

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

	class ObjectMethodDefinition {
		public:
		class_id object;
		string methodName;
		int requiredArgCount;
		vector<Variant> defaultArgs;
		virtual Variant Call(Object* obj, std::vector<Variant> args) const = 0;
	};

	template <typename R, typename T, typename... Args> requires IsDerivedFromObject<T>
	class BindedObjectMethodDefinition : public ObjectMethodDefinition {
		private:
		bool _isConst;

		public:
		R(T::* constMethod)(Args...) const = nullptr;
		R(T::* method)(Args...) = nullptr;
		
		virtual Variant Call(Object* obj, std::vector<Variant> args) const override {
			bool success = false;
			Variant result;
			if (_isConst) {
				result = call_class_method_helper<R, T, Args...>(static_cast<T*>(obj), constMethod, args, requiredArgCount, defaultArgs, success);
			}
			else {
				result = call_class_method_helper<R, T, Args...>(static_cast<T*>(obj), method, args, requiredArgCount, defaultArgs, success);
			}
			return result;
		}

		BindedObjectMethodDefinition(R(T::* met)(Args...) const) {
			constMethod = met;
			_isConst = true;
		}
		BindedObjectMethodDefinition(R(T::* met)(Args...)) {
			method = met;
			_isConst = false;
		}
	};

	class EngineClass {
		friend class type_registry;
		private:
		string _className = "";
		class_id _classId = -1;
		map<string, ObjectMethodDefinition*> _methods{};

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

		
		static class_id register_new_resource(string resource_class_name, string parent_class_name = "Object");
		static class_id get_registered_class_name(string c_name);

		static void class_register_enum(class_id c_id, string enum_name);

		template <typename R, typename T, typename... Args> requires IsDerivedFromObject<T>
		static void class_expose_method(class_id c_id, string method_name, string type, R (T::* func)(Args...)) {
			_registered_classes[c_id]._methods[method_name] = new BindedObjectMethodDefinition<R, T, Args...>(func);
		};

		// Overload to expose member functions declared const
		template <typename R, typename T, typename... Args> requires IsDerivedFromObject<T>
		static void class_expose_method(class_id c_id, string method_name, string type, R(T::* func)(Args...) const) {
			_registered_classes[c_id]._methods[method_name] = new BindedObjectMethodDefinition<R, T, Args...>(func);
		};

		static void class_define_property(class_id c_id, string prop_name, string type, string getter_method_name, string setter_method_name);

	};
}