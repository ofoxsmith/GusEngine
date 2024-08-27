#include "type_registry.h"
#include <functional>
#include <utility>

#include "project/resources/resource.h"
#include "project/resources/propertyResource.h"
#include "project/resources/shader.h"

using namespace engine_type_registry;
void type_registry::_all_types()
{
	using namespace resources;
	Resource::_register_resource();
	PropertyResource::_register_resource();
	Shader::_register_resource();
}

class_id type_registry::register_new_resource(string resource_class_name, class_id parent_class_id)
{
	return class_id();
}

class_id type_registry::get_registered_resource(string resource_class_name)
{
	return class_id();
}

void type_registry::resource_register_enum(class_id resource_class, string enum_name)
{
}
