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
		bool _isSource = false;
		string _resourcePath;
		protected:
		Resource(string type, bool isProp, string path = "");
		public:
		string Name() { return _name; }
		void SetName(string newName) { _name = newName; }
		bool IsSaved() const { return _saved; }
		bool IsSourceResource() const { return _isSource; }
		string GetPath() { return _resourcePath; }
		string Type() { return _type;  }

		virtual ~Resource() {}
	};

}