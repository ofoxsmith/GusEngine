#include "object.h"
#include "type_registry.h"

using namespace engine_type_registry;
bool Object::_IsDerivedFrom(string className)
{
	EngineClass* obj = type_registry::_registered_classes[this->_ClassName()]._inherits;
	while (obj != nullptr) {
		if (obj->_className == className) return true;
		obj = obj->_inherits;
	}
	return false;
}

map<string, ObjectRTTIModel::ObjectMethodDefinition> Object::_GetMethodList()
{
	map<string, ObjectRTTIModel::ObjectMethodDefinition> methodList = {};
	EngineClass* obj = &type_registry::_registered_classes[this->_ClassName()];
	while (obj != nullptr) {
		std::map<string, ObjectRTTIModel::ObjectMethodDefinition>::iterator it= obj->_methods.begin();

		while (it != obj->_methods.end()) {
			methodList[it->first] = it->second;
			it++;
		}

		obj = obj->_inherits;
	}

	return methodList;
}

bool Object::_HasMethod(string methodName)
{
	EngineClass* obj = &type_registry::_registered_classes[this->_ClassName()];
	while (obj != nullptr) {
		std::map<string, ObjectRTTIModel::ObjectMethodDefinition>::iterator it = obj->_methods.begin();

		while (it != obj->_methods.end()) {
			if (it->first == methodName) return true;
			it++;
		}

		obj = obj->_inherits;
	}
	return false;
}

Variant Object::_callInternal(string methodName, vector<Variant> args)
{
	EngineClass* obj = &type_registry::_registered_classes[this->_ClassName()];
	while (obj != nullptr) {
		map<string, ObjectMethod*>::iterator it = obj->_methodBinds.begin();

		while (it != obj->_methodBinds.end()) {
			if (it->first == methodName) return it->second->Call(this, args);
			it++;
		}

		obj = obj->_inherits;
	}
	return Variant(Variant::Void);
}


map<string, ObjectRTTIModel::ObjectPropertyDefinition> Object::_GetPropertyList()
{
	map<string, ObjectRTTIModel::ObjectPropertyDefinition> propertyList = {};
	EngineClass* obj = &type_registry::_registered_classes[this->_ClassName()];
	while (obj != nullptr) {
		std::map<string, ObjectRTTIModel::ObjectPropertyDefinition>::iterator it = obj->_properties.begin();

		while (it != obj->_properties.end()) {
			propertyList[it->first] = it->second;
			it++;
		}

		obj = obj->_inherits;
	}

	return propertyList;
}

bool Object::_HasProperty(string propertyName)
{
	EngineClass* obj = &type_registry::_registered_classes[this->_ClassName()];
	while (obj != nullptr) {
		std::map<string, ObjectRTTIModel::ObjectPropertyDefinition>::iterator it = obj->_properties.begin();

		while (it != obj->_properties.end()) {
			if (it->first == propertyName) return true;
			it++;
		}

		obj = obj->_inherits;
	}

	return false;
}

void Object::_Set(string propertyName, Variant value)
{
	EngineClass* obj = &type_registry::_registered_classes[this->_ClassName()];
	ObjectRTTIModel::ObjectPropertyDefinition prop;
	while (obj != nullptr) {
		std::map<string, ObjectRTTIModel::ObjectPropertyDefinition>::iterator it = obj->_properties.begin();

		while (it != obj->_properties.end()) {
			if (it->first == propertyName) {
				prop = it->second;
				break;
			}
			it++;
		}

		obj = obj->_inherits;
	}

	if (prop.isReadOnly) {
		return;
	}
	this->_Call(prop.setterName, value);
}

Variant Object::_Get(string propertyName)
{
	EngineClass* obj = &type_registry::_registered_classes[this->_ClassName()];
	ObjectRTTIModel::ObjectPropertyDefinition prop;
	while (obj != nullptr) {
		std::map<string, ObjectRTTIModel::ObjectPropertyDefinition>::iterator it = obj->_properties.begin();

		while (it != obj->_properties.end()) {
			if (it->first == propertyName) {
				prop = it->second;
				break;
			}
			it++;
		}

		obj = obj->_inherits;
	}

	return this->_Call(prop.getterName);
}
