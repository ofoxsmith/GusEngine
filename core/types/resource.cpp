#include "resource.h"
using namespace engine_type_registry;
using namespace resources;
IdGen Resource::_idGen = IdGen();
void Resource::_register_type()
{
	using namespace ObjectRTTIModel;
	engine_type_registry::type_registry::register_new_class("Resource", "Object");
	type_registry::class_expose_method(ObjectMethodDefinition("Name", Variant::StoredType::String, 0), &Resource::Name);
	type_registry::class_expose_method(ObjectMethodDefinition("SetName", Variant::StoredType::Void, 1), &Resource::SetName);
	type_registry::class_expose_method(ObjectMethodDefinition("GetPath", Variant::StoredType::String, 0), &Resource::GetPath);
	type_registry::class_expose_method(ObjectMethodDefinition("SetPath", Variant::StoredType::Void, 1), &Resource::SetPath);
	type_registry::class_expose_method(ObjectMethodDefinition("IsSaved", Variant::StoredType::Bool, 0), &Resource::IsSaved);

	ObjectRTTIModel::ObjectPropertyDefinition nameProp = ObjectPropertyDefinition("Name", Variant::StoredType::String, ObjectPropertyDefinition::NONE, "Name", "SetName");
	ObjectRTTIModel::ObjectPropertyDefinition pathProp = ObjectPropertyDefinition("Path", Variant::StoredType::String, ObjectPropertyDefinition::NONE, "GetPath", "SetPath");
	ObjectRTTIModel::ObjectPropertyDefinition savedProp = ObjectPropertyDefinition("Saved", Variant::StoredType::String, ObjectPropertyDefinition::READ_ONLY, "IsSaved");

	type_registry::class_define_property(nameProp);
	type_registry::class_define_property(pathProp);
	type_registry::class_define_property(savedProp);
	
	type_registry::end_class();

}
