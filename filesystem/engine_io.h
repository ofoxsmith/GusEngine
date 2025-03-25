#pragma once
#include "core/types/object.h"
#include "core/types/resource.h"
#include <vector>
#include <fstream>
#include <filesystem>

using namespace resources;
namespace EngineIO {
	
	class File {
		friend class FileSystem;
		private:
		std::fstream _file;
		string _path;
		string _type;
		string _name;
		File(string filePath, std::ios_base::openmode mode) {
			_path = filePath;
			_type = filesystem::path(filePath).extension().string();
			_name = filesystem::path(filePath).stem().string();
			_file = std::fstream(filePath, mode);
		}
		public:
		string FilePath() inline const {return _path; }
		string FileName() inline const {return _name; }
		string FileType() inline const {return _type; }
		string ReadAllText() {
			fstream f = fstream(_path, std::ios::ate | std::ios::in | std::ios::out);
			size_t fileSize = (size_t)f.tellg();
			std::vector<char> buffer(fileSize + 1);
			f.seekg(0);
			f.read(buffer.data(), fileSize);
			f.close();
			return string(buffer.data());
		};

		vector<uint32_t> ReadAllBinary() {
			fstream f = fstream(_path, std::ios::ate | std::ios::binary | std::ios::in | std::ios::out);
			size_t fileSize = (size_t)f.tellg();
			std::vector<char> buffer(fileSize);
			f.seekg(0);
			f.read(buffer.data(), fileSize);
			f.close();
			return { buffer.begin(), buffer.end() };
		};


		fstream* GetFileStream() {return &_file; }

		~File() {
			_file.close();
		}
	};



	class FileSystem {
		public:
		static void Init() {};
		static bool FileExists(string filePath) {
			return filesystem::exists(filePath);
		};
		static File OpenFile(string filePath, std::ios_base::openmode mode) {
			if (!FileExists(filePath)) {
				Log.Error("EngineIO", "Cannot open file: " + filePath + " - doesn't exist.");
			}
			return File(filePath, mode);
		};
		static File OpenOrCreateFile(string filePath, std::ios_base::openmode mode) {
			return File(filePath, mode);
		}
	};

	class ObjectSaver {
		public:
		static void SerialiseResourceBinary(Resource res, std::string filepath);
		static void SerialiseResourceText(Resource res, std::string filepath);
	};

	class ObjectLoader {
		private:
		static Variant LoadBinaryVariant(File* input);
		public:
		static Resource* LoadSerialisedResourceBinary(std::string filepath);
		static Resource* LoadSerialisedResourceText(std::string filepath);
	};
}