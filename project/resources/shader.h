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
		string shaderTextCode = "";
		ShaderLanguage language = LanguageGLSL;
		ShaderStage stage = StageUnknown;
	};

	class Shader: public PropertyResource {
		friend void engine_type_registry::type_registry::_all_types();
		public:

		static vector<uint32_t> CompileGLSLtoSPIRV(const std::string& source, ShaderResourceOptions::ShaderLanguage lang, ShaderResourceOptions::ShaderStage stage);
		
		Shader(ShaderResourceOptions options = {});

		VkShaderModule GetShaderModule(VkDevice device);
		vector<uint32_t> GetShaderSPIRV() const {
			return compiledCode;
		}

		void SetLanguage(ShaderResourceOptions::ShaderLanguage lng) { lang = lng; };
		ShaderResourceOptions::ShaderLanguage GetLanguage() const { return lang; };
		void SetStage(ShaderResourceOptions::ShaderStage stg) { stage = stg; };
		ShaderResourceOptions::ShaderStage GetStage() const { return stage; };

		private:
		static void _register_resource();

		vector<uint32_t> compiledCode;
		ShaderResourceOptions::ShaderLanguage lang = ShaderResourceOptions::LanguageGLSL;
		ShaderResourceOptions::ShaderStage stage = ShaderResourceOptions::StageUnknown;
	};
}