#pragma once
#include "core/globals.h"
#include <string>
namespace resources {

	class Resource
	{
		private:

		string _name;
		string _type;
		string _resourcePath;

		bool _saved = false;
		bool _isPropertyResource;

		protected:

		Resource(string type, bool isProp, string path = "");

		public:

		string Name() { return _name; }
		void SetName(string newName) { _name = newName; }
		bool IsSaved() const { return _saved; }
		bool IsPropertyResource() const { return _isPropertyResource; }
		string GetPath() { return _resourcePath; }
		string Type() { return _type;  }

		virtual ~Resource() {}
	};

}