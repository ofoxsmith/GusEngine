#include "type_registry.h"
#include <functional>
#include <utility>
#include "typeinfo"
#include "project/resources/resource.h"
#include "project/resources/propertyResource.h"
#include "project/resources/shader.h"

const int ResourceClassBits = 0b011;
const int StandardClassBits = 0b001;


using namespace engine_type_registry;
class_id type_registry::_currentId = 1;
map<class_id, EngineClass> type_registry::_registered_classes{};
map<string, class_id> type_registry::_registered_classes_names{};
void type_registry::register_all_types()
{
	using namespace resources;
	Resource::_register_type();
	PropertyResource::_register_type();
	Shader::_register_type();
}

class_id type_registry::register_new_class(string new_class_name, string parent_class_name)
{
	if (_registered_classes_names.contains(new_class_name)) {
		Log.Error("TypeRegistry", "Attempted to register class with name '" + new_class_name + "' twice.");
		return -1;
	}
	class_id id = _currentId++ | (ResourceClassBits << 29);
	EngineClass newClass{};
	newClass._className = new_class_name;
	newClass._classId = id;
	_registered_classes[id] = newClass;
	_registered_classes_names[new_class_name] = id;
	return id;
}

class_id type_registry::get_registered_class_name(string c_name) {
	return (_registered_classes_names.contains(c_name) ? _registered_classes_names[c_name] : -1);
}


void engine_type_registry::type_registry::class_define_property(class_id c_id, ClassPropertyDefinition def) {
	if (_registered_classes[c_id]._properties.contains(def.propertyName)) {
		Log.Warn("TypeRegistry", "Attempted to redefine property " + _registered_classes[c_id]._className + "::" + def.propertyName);
		return;
	}
	_registered_classes[c_id]._properties[def.propertyName] = def;
}

