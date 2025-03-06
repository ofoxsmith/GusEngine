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
		// The resource type.
		string _type = "Resource";
		// The path to a .res or .pres file if this resource is saved on disk.
		string _resourcePath = "";
		// If this resource is saved on disk.
		bool _saved = false;
		// If this resource depends on an external file
		bool _externalAsset = false;
		string _assetPath = "";

		public:

		string Name() { return _name; }
		void SetName(string newName) { _name = newName; }
		bool IsSaved() const { return _saved; }
		string GetPath() { return _resourcePath; }
		void SetPath(string path) { _resourcePath = path; }
		string Type() { return _type; }

		bool IsExternalAsset() const { return _externalAsset; }
		string GetAssetPath() const { return _assetPath; }

		Resource() {}
		virtual ~Resource() {}
	};
}