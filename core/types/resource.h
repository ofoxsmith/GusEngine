#pragma once
#include "core/globals.h"
#include "core/types/type_registry.h"
#include "core/types/object.h"
#include <string>
namespace resources {

	class Resource : public Object
	{
		GUS_DECLARE_CLASS(Resource, Object)
		protected:
		// The name of the resource.
		string _name = "";
		// The path to a .res or .pres file if this resource is saved on disk.
		string _resourcePath = "";
		// If this resource is saved on disk.
		bool _saved = false;

		public:

		string Name() { return _name; }
		void SetName(string newName) { _name = newName; }
		bool IsSaved() const { return _saved; }
		string GetPath() { return _resourcePath; }
		void SetPath(string path) { _resourcePath = path; }

		Resource() {}
		virtual ~Resource() {}
	};
}