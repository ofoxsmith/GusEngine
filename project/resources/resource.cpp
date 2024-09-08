#include "resource.h"
#include "core/types/type_registry.h"
using namespace engine_type_registry;
using namespace resources;
void Resource::_register_resource(class_id cId)
{
	type_registry::class_expose_method(cId, ObjectMethodDescription("Name", Variant::String, 0), &Resource::Name);
	type_registry::class_expose_method(cId, ObjectMethodDescription("SetName", Variant::Void, 1), &Resource::SetName);
	type_registry::class_expose_method(cId, ObjectMethodDescription("SetPath", Variant::Void, 1), &Resource::SetPath);
	type_registry::class_expose_method(cId, ObjectMethodDescription("IsSaved", Variant::Bool, 0), &Resource::IsSaved);
	type_registry::class_expose_method(cId, ObjectMethodDescription("GetPath", Variant::String, 0), &Resource::GetPath);
	type_registry::class_expose_method(cId, ObjectMethodDescription("SetPath", Variant::Void, 1), &Resource::SetPath);
	type_registry::class_expose_method(cId, ObjectMethodDescription("Type", Variant::String, 0), &Resource::Type);

}
