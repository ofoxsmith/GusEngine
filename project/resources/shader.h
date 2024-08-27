#pragma once
#include "project/resources/propertyResource.h"
#include "filesystem/file_helpers.h"
#include <vector>
#include <vulkan/vulkan.h>
namespace resources {
	struct ShaderResourceOptions : public PropertyResourceOptions {
		enum ShaderLanguage {
			LanguageGLSL,
			LanguageHLSL
		};
		enum ShaderStage {
			StageUnknown,
			StageVert,
			StageFrag,
			StageTessControl,
			StageTessEval,
			StageGeom,
			StageComp
		};

		std::vector<uint32_t> spirvBinary{};
		string shaderCode = "";
		ShaderLanguage shaderLanguage = LanguageGLSL;
		ShaderStage shaderStage = StageUnknown;
	};

	class Shader: public PropertyResource {
		public:

		static vector<uint32_t> CompileGLSLtoSPIRV(const std::string& source, ShaderResourceOptions::ShaderLanguage lang, ShaderResourceOptions::ShaderStage stage);
		
		Shader(ShaderResourceOptions options = {});

		VkShaderModule GetShaderModule(VkDevice device);
		vector<uint32_t> GetShaderSPIRV() const {
			return compiledCode;
		}

		private:

		vector<uint32_t> compiledCode;
		ShaderResourceOptions::ShaderLanguage lang = ShaderResourceOptions::LanguageGLSL;
		ShaderResourceOptions::ShaderStage stage = ShaderResourceOptions::StageUnknown;
	};
}