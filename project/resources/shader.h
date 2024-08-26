#pragma once
#include "project/resources/propertyResource.h"
#include "filesystem/file_helpers.h"
#include <vector>
namespace resources {
	class Shader: public PropertyResource {
		private:

		static vector<uint32_t> _compileGLSLtoSPIRV(const std::string& source, string type);
		vector<uint32_t> compiledCode;

		public:

		Shader(ParsedPropertyResourceFile data);
		vector<uint32_t> GetShaderSPIRV() const {
			return compiledCode;
		}

		void Init();
	};
}