#include "propertyResource.h"
#include "core/type_registry.h"
void resources::PropertyResource::_register_resource()
{
	using namespace engine_type_registry;
	class_id cId = type_registry::register_new_resource("PropertyResource", type_registry::get_registered_resource("Resource"));
	type_registry::resource_define_property(cId, "sourceFilePath", "string", &PropertyResource::SetSourceFilepath);
}
