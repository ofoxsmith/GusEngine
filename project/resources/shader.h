#pragma once
#include "core/types/resource.h"
#include <vector>
#include <vulkan/vulkan.h>
namespace resources {
	class Shader: public Resource {
		GUS_DECLARE_CLASS(Shader, Resource)
		public:

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

		static Shader* Create(const std::string& source, ShaderLanguage lang, ShaderStage stage);
		
		Shader() {};

		~Shader();
		virtual void _Init() override {

		}

		VkShaderModule GetShaderModule(VkDevice device);

		vector<uint32_t> GetShaderSPIRV() const {
			return _spirvBinary;
		}

		ShaderLanguage GetLanguage() const { return _lang; };
		ShaderStage GetStage() const { return _stage; };

		private:
		VkShaderModule _shaderModule = nullptr;
		vector<uint32_t> _spirvBinary;
		ShaderLanguage _lang = ShaderLanguage::LanguageGLSL;
		ShaderStage _stage = ShaderStage::StageUnknown;
	};
}