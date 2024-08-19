#pragma once
#include "core/globals.h"
#include <string>
namespace resources {

	class Resource
	{
		private:
		string _name;
		string _type;
		bool _saved = false;
		bool _hasSource = false;
		string _resourcePath;
		string _resourceSourcePath;
		protected:
		Resource(string type, string path="", string sourcePath="");
		public:
		string Name() { return _name; }
		void SetName(string newName) { _name = newName; }
		bool IsSaved() const { return _saved; }
		bool HasSourceFile() const { return _hasSource; }
		string GetPath() { return _resourcePath; }
		string GetSourcePath() { return _resourceSourcePath; }
		string Type() { return _type;  }

		virtual ~Resource() {}
	};

}