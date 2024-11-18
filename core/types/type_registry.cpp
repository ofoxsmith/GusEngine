#include "type_registry.h"
#include <functional>
#include <utility>
#include "typeinfo"
#include "core/types/resource.h"
#include "project/resources/propertyResource.h"
#include "project/resources/shader.h"

using namespace engine_type_registry;
map<string, EngineClass> type_registry::_registered_classes{};
void type_registry::register_all_types()
{
	using namespace resources;
	Resource::_register_type();
	PropertyResource::_register_type();
	Shader::_register_type();
}

void type_registry::register_new_class(string new_class_name, string parent_class_name)
{
	if (_registered_classes.contains(new_class_name)) {
		Log.Error("TypeRegistry", "Attempted to register class with name '" + new_class_name + "' twice.");
		return;
	}
	EngineClass newClass{};
	newClass._className = new_class_name;
	_registered_classes[new_class_name] = newClass;
}

void engine_type_registry::type_registry::class_define_property(string class_name, ObjectRTTIModel::ObjectPropertyDefinition def) {
	if (_registered_classes[class_name]._properties.contains(def.propertyName)) {
		Log.Warn("TypeRegistry", "Attempted to redefine property " + class_name + "::" + def.propertyName);
		return;
	}
	_registered_classes[class_name]._properties[def.propertyName] = def;
}

