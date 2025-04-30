#include "image.h"

void resources::Image::_register_type() {
	using namespace engine_type_registry;
	using namespace ObjectRTTIModel;

	type_registry::register_new_class("Image", "Resource");
}