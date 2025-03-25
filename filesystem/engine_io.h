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
		File(string filePath) {
			_path = filePath;
			_type = filesystem::path(filePath).extension().string();
			_name = filesystem::path(filePath).stem().string();
			_file = std::fstream(filePath, std::ios::binary | std::ios::in | std::ios::out);
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

		///Read functions
		void Get(char* str, int count) {
			_file.get(str, count);
		};
		void Peek(char* str, int count) {
			int current = _file.tellg();
			_file.get(str, count);
			_file.seekg(current);
		};
		int GetReadPos() { return _file.tellg(); }
		void SeekReadPos(int pos) { _file.seekg(pos); }

		//Write functions
		int GetWritePos() { return _file.tellp(); }
		void SeekWritePos(int pos) { _file.seekp(pos); }
		void Write(const char* str, int count) {
			_file.write(str, count);
		};
		void WriteByte(const char b) {_file.put(b);}
	};



	class FileSystem {
		public:
		static void Init() {};
		static bool FileExists(string filePath) {
			return filesystem::exists(filePath);
		};
		static File OpenFile(string filePath) {
			return File(filePath);
		};
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