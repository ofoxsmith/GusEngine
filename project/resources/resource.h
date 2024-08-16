#pragma once
#include "filesystem/file_helpers.h"
#include "core/globals.h"
#include <string>
namespace resources {

	class Resource
	{
		private:
		string name;
		string resourceType;
		string resourcePath;
		string resourceSourcePath;
		protected:
		Resource(string type, string path);
		public:
		string Name() { return name; }
		void SetName(string newName) { name = newName; }
		string GetPath() { return resourcePath; }
		string Type() { return resourceType;  }

		virtual ~Resource() {}
	};

}