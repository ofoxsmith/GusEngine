#pragma once
#include "project/resources/resource.h"
#include <vector>
namespace resources {
	class Shader: public Resource {
		private:
		std::vector<char> compiledCode;
		public:
		Shader(const string path);
		std::vector<char> GetShaderSPIRV() {
			return compiledCode;
		}
	};
}