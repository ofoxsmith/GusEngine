#include "propertyResource.h"
#include "core/types/type_registry.h"
using namespace engine_type_registry;
void resources::PropertyResource::_register_resource(class_id cId)
{
	type_registry::class_expose_method(cId, ObjectMethodDescription("SetSourceFilepath", Variant::Void, 1), &PropertyResource::SetSourceFilepath);
	type_registry::class_expose_method(cId, ObjectMethodDescription("GetSourceFilepath", Variant::String, 0), &PropertyResource::GetSourceFilepath);
}
