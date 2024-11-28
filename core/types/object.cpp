#include "object.h"
#include "type_registry.h"

bool Object::_IsDerivedFrom(string className)
{
	return false;
}

map<string, ObjectRTTIModel::ObjectMethodDefinition> Object::_GetMethodList()
{
	return engine_type_registry::type_registry::_registered_classes[this->_ClassName()]._methods;
}

bool Object::_HasMethod(string methodName)
{
	return engine_type_registry::type_registry::_registered_classes[this->_ClassName()]._methods.contains(methodName);
}

Variant Object::_callInternal(string methodName, vector<Variant> args)
{
	return engine_type_registry::type_registry::_registered_classes[this->_ClassName()]._methodBinds[methodName]->Call(this, args);
}


map<string, ObjectRTTIModel::ObjectPropertyDefinition> Object::_GetPropertyList()
{
	return engine_type_registry::type_registry::_registered_classes[this->_ClassName()]._properties;
}

bool Object::_HasProperty(string propertyName)
{
	return engine_type_registry::type_registry::_registered_classes[this->_ClassName()]._properties.contains(propertyName);
}

void Object::_Set(string propertyName, Variant value)
{
	ObjectRTTIModel::ObjectPropertyDefinition prop = engine_type_registry::type_registry::_registered_classes[this->_ClassName()]._properties[propertyName];
	if (prop.isReadOnly) {
		return;
	}
	this->_Call(prop.setterName, value);
}

Variant Object::_Get(string propertyName)
{
	ObjectRTTIModel::ObjectPropertyDefinition prop = engine_type_registry::type_registry::_registered_classes[this->_ClassName()]._properties[propertyName];
	return this->_Call(prop.getterName);
}
