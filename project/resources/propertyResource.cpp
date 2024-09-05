#include "propertyResource.h"
#include "core/types/type_registry.h"
using namespace engine_type_registry;
void resources::PropertyResource::_register_resource(class_id cId)
{
	type_registry::class_expose_method(cId, "SetSourceFilepath", "string", &PropertyResource::SetSourceFilepath);
	type_registry::class_expose_method(cId, "GetSourceFilepath", "string", &PropertyResource::GetSourceFilepath);
}
