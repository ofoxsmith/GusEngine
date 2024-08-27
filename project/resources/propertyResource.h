#pragma once
#include "core/globals.h"
#include "project/resources/resource.h"
#include <map>
namespace resources {

	struct PropertyResourceOptions: public ResourceOptions {
		string sourceFilePath = "";
	};

	class PropertyResource : public Resource {
		friend void engine_type_registry::type_registry::_all_types();
		public:
		PropertyResource(PropertyResourceOptions opts = {}) : Resource(opts) {
			_type = "PropertyResource";
			_sourcePath = opts.sourceFilePath;
		}

		// Intermediate structure used when reading a property resource from disk.
		struct PropertyResourceSourceLoader {
			string type{};
			unsigned int start{};
			unsigned int end{};
		};
		struct ParsedPropertyResourceFile {
			string resourceName;
			string resourcePath;
			string dataPath;
			PropertyResourceSourceLoader dataLoader;
			string mainSectionType;
			// Values which can be read as integers, stored as [key: 0123]
			map<string, int> mainSectionIntData;
			// Values which can be read as strings, stored as [key: "value"]
			map<string, string> mainSectionStringData;
			// Values which must be specifically parsed by a resource constructor, stored as [key: value]
			map<string, string> mainSectionUnprocessedData;
		};

		void SetSourceFilepath(string path) { _sourcePath = path; }
		string GetSourceFilepath() { return _sourcePath; }

		private:
		static void _register_resource();

		protected:
		// The path to the data file encapsulated in this resource.
		string _sourcePath = "";

	};

}