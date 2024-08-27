#include "resource.h"
#include "core/type_registry.h"
void resources::Resource::_register_resource()
{
	using namespace engine_type_registry;
	class_id cId = type_registry::register_new_resource("Resource");
	type_registry::resource_define_property(cId, "resourceName", "string", &Resource::SetName);
	type_registry::resource_define_property(cId, "resourceSavedPath", "string", &Resource::SetPath);
}