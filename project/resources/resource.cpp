#include "resource.h"
#include "core/types/type_registry.h"
using namespace engine_type_registry;
using namespace resources;
void Resource::_register_resource(class_id cId)
{
	type_registry::class_expose_method(cId, "Name", "string", &Resource::Name);
	type_registry::class_expose_method(cId, "SetName", "string", &Resource::SetName);
	type_registry::class_expose_method(cId, "SetPath", "string", &Resource::SetPath);
	type_registry::class_expose_method(cId, "IsSaved", "bool", &Resource::IsSaved);
	type_registry::class_expose_method(cId, "GetPath", "string", &Resource::GetPath);
	type_registry::class_expose_method(cId, "SetPath", "string", &Resource::SetPath);
	type_registry::class_expose_method(cId, "Type", "string", &Resource::Type);

}
