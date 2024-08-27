#pragma once
#include <string>
using namespace std;
namespace engine_type_registry {
	typedef int class_id;
	class type_registry {
		private:
		public:
		static void _all_types();

		static class_id register_new_resource(string resource_class_name, class_id parent_class_name = -1);
		static class_id get_registered_resource(string resource_class_name);

		static void resource_register_enum(class_id resource_class, string enum_name);

		template <typename T, typename... Args>
		static void resource_define_property(class_id resource_class, string prop_name, string type, void (T::* setter)(Args...)) {};

	};
}