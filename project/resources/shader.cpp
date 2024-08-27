#include "shader.h"
#include <glslang/Public/ShaderLang.h>
#include <glslang/SPIRV/GlslangToSpv.h>
#include <glslang/Public/ResourceLimits.h>
#include "filesystem/engine_data_cache.h"
resources::Shader::Shader(ParsedPropertyResourceFile data): PropertyResource("Shader", data) {

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

void resources::Shader::Init() {
	if (file_helpers::get_file_type(_sourcePath) == "spv") {
		auto spv = EngineDataCache::LoadFileBinary(_sourcePath);
		compiledCode = spv;
	}
	else {
		string data = EngineDataCache::LoadFileText(_sourcePath);
		vector<uint32_t> output = _compileGLSLtoSPIRV(data, file_helpers::get_file_type(_sourcePath));
		compiledCode = output;
	}
}

vector<uint32_t> resources::Shader::_compileGLSLtoSPIRV(const std::string& source, string type)
{

	std::vector<uint32_t> spirv;
	std::string	info_log;
	glslang::InitializeProcess();

	auto messages = static_cast<EShMessages>(EShMsgReadHlsl | EShMsgDefault | EShMsgVulkanRules | EShMsgSpvRules);

	EShLanguage stage{};

	if (type == "vert") stage = EShLangVertex;
	if (type == "frag") stage = EShLangFragment;
	if (type == "tesc") stage = EShLangTessControl;
	if (type == "tese") stage = EShLangTessEvaluation;
	if (type == "geom") stage = EShLangGeometry;
	if (type == "comp") stage = EShLangCompute;

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
