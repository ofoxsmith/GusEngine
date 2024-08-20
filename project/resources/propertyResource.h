#pragma once
#include "core/globals.h"
#include "project/resources/resource.h"

namespace resources {
	class PropertyResource : public Resource {
	private:
	protected:
		string _sourcePath;
		PropertyResource(string type, string sourcePath, string path = "");
	public:
		string GetSourcePath() { return _sourcePath; }
		void ReloadSourceFile();
		virtual void Init() = 0;
	};
}