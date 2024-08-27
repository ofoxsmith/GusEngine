#pragma once
#include "core/globals.h"
#include <string>
namespace resources {
	struct ResourceOptions {
		string resourceName = "";
		string resourceSavedPath = "";
	};

	class Resource
	{
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
		string Type() { return _type; }

		Resource(ResourceOptions opts = {}) {
			_name = opts.resourceName;
			_resourcePath = opts.resourceSavedPath;
			_saved = opts.resourceSavedPath != "";
		}
		virtual ~Resource() {}
	};
}