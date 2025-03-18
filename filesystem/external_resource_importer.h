#pragma once
#include "core/globals.h"
#include <string>
#include "unordered_map"
#include <fstream>
#include "core/types/resource.h"
// A class that allows for project resources and data to be stored in an engine-readable format, rather than converting/importing 
// them into engine-readable data every time the project is restarted.
class EngineDataCache {
public:
	void Init();
	void Cleanup();

	bool IsResourceImported(string filePath);

	void ReloadResource(string filePath);
	void ImportResource(string filePath);
private:
	std::unordered_map<string, std::tuple<int, string>> savedHashes{};
	std::fstream hashFile{};
	void SaveMD5Hash(string filePath, string hash);
    string GetFileMD5Hash(std::string fileName);
	bool HasMD5HashChanged(std::string filePath, std::string newHash);
};
