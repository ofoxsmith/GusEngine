#pragma once
#include "core/types/resource.h"
#include <vector>
#include <vulkan/vulkan.h>
namespace resources {
	struct ShaderResourceOptions {
		enum class ShaderLanguage {
			LanguageGLSL,
			LanguageHLSL
		};
		enum class ShaderStage {
			StageUnknown,
			StageVert,
			StageFrag,
			StageTessControl,
			StageTessEval,
			StageGeom,
			StageComp
		};

		std::vector<unsigned int> spirvBinary{};
		string shaderTextCode = "";
		ShaderLanguage language = ShaderLanguage::LanguageGLSL;
		ShaderStage stage = ShaderStage::StageUnknown;
	};

	class Shader: public Resource {
		GUS_DECLARE_CLASS(Shader, Resource)
		public:

		static vector<unsigned int> CompileGLSLtoSPIRV(const std::string& source, ShaderResourceOptions::ShaderLanguage lang, ShaderResourceOptions::ShaderStage stage);
		
		Shader() {};

		~Shader();
		virtual void _Init() override {

		}

		VkShaderModule GetShaderModule(VkDevice device);
		void SetShaderSPIRV(vector<unsigned int> spirv) {
			compiledCode = spirv;
		}
		vector<unsigned int> GetShaderSPIRV() const {
			return compiledCode;
		}

		void SetLanguage(ShaderResourceOptions::ShaderLanguage lng) { lang = lng; };
		ShaderResourceOptions::ShaderLanguage GetLanguage() const { return lang; };
		void SetStage(ShaderResourceOptions::ShaderStage stg) { stage = stg; };
		ShaderResourceOptions::ShaderStage GetStage() const { return stage; };

		private:
		VkShaderModule _shaderModule = nullptr;
		vector<unsigned int> compiledCode;
		ShaderResourceOptions::ShaderLanguage lang = ShaderResourceOptions::ShaderLanguage::LanguageGLSL;
		ShaderResourceOptions::ShaderStage stage = ShaderResourceOptions::ShaderStage::StageUnknown;
	};
}