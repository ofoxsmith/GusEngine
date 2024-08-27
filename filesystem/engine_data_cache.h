#pragma once
#include "core/globals.h"
#include <string>
#include "unordered_map"
#include <fstream>
// A class that allows for project resources and data to be cached in a binary format, rather than read from text every time
// the project restarts or a file is reloaded.
class EngineDataCache {
	public:
		static void Init();
		static void Cleanup();
		static vector<uint32_t> LoadFileBinary(string filePath);
		static string LoadFileText(string filePath);
		static bool LoadFileFromCacheOnly(string filePath);
		static void RemoveFileFromCache(string filePath);
		static void ReloadCachedFile(string filePath);
    // Returns a 128-bit hash as a 32 char string
private:
	static std::unordered_map<string, std::tuple<int, string>> savedHashes;
	static std::fstream hashFile;
	static void SaveMD5Hash(string filePath, string hash);
    static string GetFileMD5Hash(std::string fileName);
	static bool HasMD5HashChanged(std::string filePath, std::string newHash);
};
