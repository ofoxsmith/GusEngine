#include "propertyResource.h"
#include "core/types/type_registry.h"
using namespace engine_type_registry;
void resources::PropertyResource::_register_type()
{
	type_registry::register_new_class("PropertyResource", "Resource");
	type_registry::class_expose_method("PropertyResource", ObjectRTTIModel::ObjectMethodDefinition("SetSourceFilepath", Variant::StoredType::Void, 1), &PropertyResource::SetSourceFilepath);
	type_registry::class_expose_method("PropertyResource", ObjectRTTIModel::ObjectMethodDefinition("GetSourceFilepath", Variant::StoredType::String, 0), &PropertyResource::GetSourceFilepath);
}
