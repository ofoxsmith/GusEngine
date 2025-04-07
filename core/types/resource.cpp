#include "resource.h"
using namespace engine_type_registry;
using namespace resources;
void Resource::_register_type()
{
	using namespace ObjectRTTIModel;
	engine_type_registry::type_registry::register_new_class("Resource", "Object");
	type_registry::class_expose_method("Resource", ObjectMethodDefinition("Name", Variant::StoredType::String, 0), &Resource::Name);
	type_registry::class_expose_method("Resource", ObjectMethodDefinition("SetName", Variant::StoredType::Void, 1), &Resource::SetName);
	type_registry::class_expose_method("Resource", ObjectMethodDefinition("GetPath", Variant::StoredType::String, 0), &Resource::GetPath);
	type_registry::class_expose_method("Resource", ObjectMethodDefinition("SetPath", Variant::StoredType::Void, 1), &Resource::SetPath);
	type_registry::class_expose_method("Resource", ObjectMethodDefinition("IsSaved", Variant::StoredType::Bool, 0), &Resource::IsSaved);

	ObjectRTTIModel::ObjectPropertyDefinition nameProp = ObjectPropertyDefinition("Name", Variant::StoredType::String, ObjectPropertyDefinition::NONE, "Name", "SetName");
	ObjectRTTIModel::ObjectPropertyDefinition pathProp = ObjectPropertyDefinition("Path", Variant::StoredType::String, ObjectPropertyDefinition::NONE, "GetPath", "SetPath");
	ObjectRTTIModel::ObjectPropertyDefinition savedProp = ObjectPropertyDefinition("Saved", Variant::StoredType::String, ObjectPropertyDefinition::READ_ONLY, "IsSaved");

	type_registry::class_define_property("Resource", nameProp);
	type_registry::class_define_property("Resource", pathProp);
	type_registry::class_define_property("Resource", savedProp);

}
