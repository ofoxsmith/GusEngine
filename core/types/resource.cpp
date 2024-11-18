#include "resource.h"
using namespace engine_type_registry;
using namespace resources;
void Resource::_register_type()
{
	engine_type_registry::type_registry::register_new_class("Resource", "Object");
	type_registry::class_expose_method("Resource", ObjectRTTIModel::ObjectMethodDefinition("Name", Variant::StoredType::String, 0), &Resource::Name);
	type_registry::class_expose_method("Resource", ObjectRTTIModel::ObjectMethodDefinition("SetName", Variant::StoredType::Void, 1), &Resource::SetName);
	type_registry::class_expose_method("Resource", ObjectRTTIModel::ObjectMethodDefinition("GetPath", Variant::StoredType::String, 0), &Resource::GetPath);
	type_registry::class_expose_method("Resource", ObjectRTTIModel::ObjectMethodDefinition("SetPath", Variant::StoredType::Void, 1), &Resource::SetPath);
	type_registry::class_expose_method("Resource", ObjectRTTIModel::ObjectMethodDefinition("IsSaved", Variant::StoredType::Bool, 0), &Resource::IsSaved);
	type_registry::class_expose_method("Resource", ObjectRTTIModel::ObjectMethodDefinition("Type", Variant::StoredType::String, 0), &Resource::Type);

	ObjectRTTIModel::ObjectPropertyDefinition nameProp = ObjectRTTIModel::ObjectPropertyDefinition("Name", Variant::StoredType::String, false, "Name", "SetName");
	ObjectRTTIModel::ObjectPropertyDefinition pathProp = ObjectRTTIModel::ObjectPropertyDefinition("Path", Variant::StoredType::String, false, "GetPath", "SetPath");
	ObjectRTTIModel::ObjectPropertyDefinition savedProp = ObjectRTTIModel::ObjectPropertyDefinition("Saved", Variant::StoredType::String, true, "IsSaved");
	ObjectRTTIModel::ObjectPropertyDefinition typeProp = ObjectRTTIModel::ObjectPropertyDefinition("Type", Variant::StoredType::String, true, "Type");

	type_registry::class_define_property("Resource", nameProp);
	type_registry::class_define_property("Resource", pathProp);
	type_registry::class_define_property("Resource", savedProp);
	type_registry::class_define_property("Resource", typeProp);

}
