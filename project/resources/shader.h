#pragma once
#include "project/resources/resource.h"
#include <vector>
namespace resources {
	class Shader: public Resource {
		private:
		vector<uint32_t> compiledCode;
		public:
		Shader(vector<uint32_t> source, const string path, const string sourceFilepath);
		vector<uint32_t> GetShaderSPIRV() const {
			return compiledCode;
		}
	};
}