#pragma once
#include "project/resources/resource.h"
namespace resources {
	class Shader: public Resource {
		private:
		vector<char> compiledCode;
		public:
		Shader(const string path) : Resource("Shader", path) {
			std::cout << path << endl;
			compiledCode = file_helpers::read_file(path);
		}

		vector<char> GetShaderSPIRV() {
			return compiledCode;
		}
	};
}