#pragma once
#include "core/globals.h"
#include "project/resources/resource.h"
#include <map>
namespace resources {
	class PropertyResource : public Resource {
		public:
		
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

		string GetSourceFilepath() { return _sourcePath; }
		virtual void Init() = 0;

		private:

		protected:
		string _sourcePath;
		PropertyResource(string type, ParsedPropertyResourceFile data);

	};

}