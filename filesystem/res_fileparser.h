#pragma once
#include "core/globals.h"
#include "file_helpers.h"
#include <string>
#include <map>
class ResourceFileParser {
public:
	struct PropertyResourceSourceLoader {
		string type{};
		unsigned int start{};
		unsigned int end{};
	};
	struct ParsedPropertyResourceFile {
		string resourceName;
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
	static void LoadResource(const string filePath);
	static ParsedPropertyResourceFile LoadPropResource(const string filePath);
	void LoadSceneResource() { throw new runtime_error("not implemented"); }
};