#pragma once
#include "core/globals.h"
#include <string>
#include "unordered_map"
#include <fstream>
// A class that allows for project resources and data to be cached in a binary format, rather than read from text every time
// the project restarts or a file is reloaded.
class EngineDataCache {
public:
	void Init();
	void Cleanup();
	vector<uint32_t> LoadFileBinary(string filePath);
	string LoadFileText(string filePath);
	bool LoadFileFromCacheOnly(string filePath);
	void RemoveFileFromCache(string filePath);
	void ReloadCachedFile(string filePath);
private:
	std::unordered_map<string, std::tuple<int, string>> savedHashes{};
	std::fstream hashFile{};
	void SaveMD5Hash(string filePath, string hash);
    string GetFileMD5Hash(std::string fileName);
	bool HasMD5HashChanged(std::string filePath, std::string newHash);
};
