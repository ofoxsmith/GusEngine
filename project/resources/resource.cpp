#include "resource.h"
using namespace engine_type_registry;
using namespace resources;
void Resource::_register_type()
{
	engine_type_registry::class_id cId = engine_type_registry::type_registry::register_new_class("Resource", "Object");
	type_registry::class_expose_method(cId, ClassMethodDescription("Name", Variant::StoredType::String, 0), &Resource::Name);
	type_registry::class_expose_method(cId, ClassMethodDescription("SetName", Variant::StoredType::Void, 1), &Resource::SetName);
	type_registry::class_expose_method(cId, ClassMethodDescription("GetPath", Variant::StoredType::String, 0), &Resource::GetPath);
	type_registry::class_expose_method(cId, ClassMethodDescription("SetPath", Variant::StoredType::Void, 1), &Resource::SetPath);
	type_registry::class_expose_method(cId, ClassMethodDescription("IsSaved", Variant::StoredType::Bool, 0), &Resource::IsSaved);
	type_registry::class_expose_method(cId, ClassMethodDescription("Type", Variant::StoredType::String, 0), &Resource::Type);

	ClassPropertyDefinition nameProp = ClassPropertyDefinition("Name", Variant::StoredType::String, false, "Name", "SetName");
	ClassPropertyDefinition pathProp = ClassPropertyDefinition("Path", Variant::StoredType::String, false, "GetPath", "SetPath");
	ClassPropertyDefinition savedProp = ClassPropertyDefinition("Saved", Variant::StoredType::String, true, "IsSaved");
	ClassPropertyDefinition typeProp = ClassPropertyDefinition("Type", Variant::StoredType::String, true, "Type");

	type_registry::class_define_property(cId, nameProp);
	type_registry::class_define_property(cId, pathProp);
	type_registry::class_define_property(cId, savedProp);
	type_registry::class_define_property(cId, typeProp);

}
