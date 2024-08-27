#include "shader.h"
#include <glslang/Public/ShaderLang.h>
#include <glslang/SPIRV/GlslangToSpv.h>
#include <glslang/Public/ResourceLimits.h>

resources::Shader::Shader(ShaderResourceOptions opts): PropertyResource(opts) {
	_type = "Shader";

	if (opts.shaderCode == "" && opts.spirvBinary.empty()) {
		throw new runtime_error("Failed to create shader resource: Both opts.shaderCode and opts.spirvBinary were empty.");
	}
	stage = opts.shaderStage;
	lang = opts.shaderLanguage;
	if (!opts.spirvBinary.empty()) {
		compiledCode = opts.spirvBinary;
	}
	else {
		compiledCode = CompileGLSLtoSPIRV(opts.shaderCode, opts.shaderLanguage, opts.shaderStage);
	}
}

VkShaderModule resources::Shader::GetShaderModule(VkDevice device)
{
	VkShaderModuleCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.codeSize = 4 * compiledCode.size();
	createInfo.pCode = compiledCode.data();
	VkShaderModule shaderModule;
	if (vkCreateShaderModule(device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
		Log.FatalError("Vulkan", "Failed to create shader module.");
	}
	return shaderModule;

}

vector<uint32_t> resources::Shader::CompileGLSLtoSPIRV(const std::string& source, ShaderResourceOptions::ShaderLanguage lang, ShaderResourceOptions::ShaderStage type)
{

	std::vector<uint32_t> spirv;
	std::string	info_log;
	glslang::InitializeProcess();

	auto messages = static_cast<EShMessages>(EShMsgReadHlsl | EShMsgDefault | EShMsgVulkanRules | EShMsgSpvRules);

	EShLanguage stage{};

	if (type == ShaderResourceOptions::StageVert) stage = EShLangVertex;
	if (type == ShaderResourceOptions::StageFrag) stage = EShLangFragment;
	if (type == ShaderResourceOptions::StageTessControl) stage = EShLangTessControl;
	if (type == ShaderResourceOptions::StageTessEval) stage = EShLangTessEvaluation;
	if (type == ShaderResourceOptions::StageGeom) stage = EShLangGeometry;
	if (type == ShaderResourceOptions::StageComp) stage = EShLangCompute;

	const char* shader_source = reinterpret_cast<const char*>(source.data());

	glslang::TShader shader(stage);
	shader.setStringsWithLengths(&shader_source, nullptr, 1);
	shader.setEnvInput(glslang::EShSourceGlsl, stage, glslang::EShClientVulkan, 1);
	shader.setEntryPoint("main");
	shader.setSourceEntryPoint("main");
	shader.setEnvClient(glslang::EShClientVulkan, glslang::EShTargetVulkan_1_3);
	shader.setEnvTarget(glslang::EshTargetSpv, glslang::EShTargetSpv_1_6);

	if (!shader.parse(GetDefaultResources(), 100, false, messages))
	{
		Log.Error("ResourceLoader: GLSLang", "Failed to parse GLSL shader, Error:" + std::string(shader.getInfoLog()) + "\n" + std::string(shader.getInfoDebugLog()));
		return {};
	}

	glslang::TProgram program;
	program.addShader(&shader);

	if (!program.link(messages))
	{
		Log.Error("ResourceLoader: GLSLang", "Failed to compile GLSL shader, Error:" + std::string(shader.getInfoLog()) + "\n" + std::string(shader.getInfoDebugLog()));
		return {};
	}

	glslang::TIntermediate* intermediate = program.getIntermediate(stage);
	if (!intermediate)
	{
		Log.Error("ResourceLoader: GLSLang", "Failed to get generated SPIRV, Error:" + std::string(shader.getInfoLog()) + "\n" + std::string(shader.getInfoDebugLog()));
		return {};
	}

	spv::SpvBuildLogger logger;

	glslang::GlslangToSpv(*intermediate, spirv, &logger);

	glslang::FinalizeProcess();
	return spirv;
}
