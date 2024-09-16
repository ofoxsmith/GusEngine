#include "propertyResource.h"
#include "core/types/type_registry.h"
using namespace engine_type_registry;
void resources::PropertyResource::_register_type()
{
	class_id cId = type_registry::register_new_class("PropertyResource", "Resource");
	type_registry::class_expose_method(cId, ClassMethodDescription("SetSourceFilepath", Variant::StoredType::Void, 1), &PropertyResource::SetSourceFilepath);
	type_registry::class_expose_method(cId, ClassMethodDescription("GetSourceFilepath", Variant::StoredType::String, 0), &PropertyResource::GetSourceFilepath);
}
