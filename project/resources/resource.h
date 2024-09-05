#pragma once
#include "core/globals.h"
#include "core/types/object.h"
#include <string>
namespace resources {
	struct ResourceOptions {
		string resourceName = "";
		string resourceSavedPath = "";
	};
	class Resource : public Object
	{
		GUS_CLASS(Resource)
		protected:
		// The name of the resource.
		string _name = "";
		// The resource type.
		string _type = "Resource";
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
		string Type() { return _type; }

		Resource(ResourceOptions opts = {}) {
			_name = opts.resourceName;
			_resourcePath = opts.resourceSavedPath;
			_saved = opts.resourceSavedPath != "";
		}
		virtual ~Resource() {}
	};
}