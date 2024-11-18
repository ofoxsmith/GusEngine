#include "shader.h"
#include <glslang/Public/ShaderLang.h>
#include <glslang/SPIRV/GlslangToSpv.h>
#include <glslang/Public/ResourceLimits.h>
#include "core/types/type_registry.h"

resources::Shader::Shader(ShaderResourceOptions opts): PropertyResource(opts) {
	_type = "Shader";

	if (opts.shaderTextCode == "" && opts.spirvBinary.empty()) {
		throw new runtime_error("Failed to create shader resource: Both opts.shaderTextCode and opts.spirvBinary were empty.");
	}
	stage = opts.stage;
	lang = opts.language;
	if (!opts.spirvBinary.empty()) {
		compiledCode = opts.spirvBinary;
	}
	else {
		compiledCode = CompileGLSLtoSPIRV(opts.shaderTextCode, opts.language, opts.stage);
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

void resources::Shader::_register_type() {
	using namespace engine_type_registry;
	type_registry::register_new_class("Shader", "PropertyResource");
	//type_registry::class_expose_method("Shader", "GetShaderSPIRV", "vector<uint32_t>", &Shader::GetShaderSPIRV);
	type_registry::class_expose_method("Shader", ObjectRTTIModel::ObjectMethodDefinition("SetLanguage", Variant::StoredType::Void), &Shader::SetLanguage);
	type_registry::class_expose_method("Shader", ObjectRTTIModel::ObjectMethodDefinition("GetLanguage", Variant::StoredType::Int), &Shader::GetLanguage);
	type_registry::class_expose_method("Shader", ObjectRTTIModel::ObjectMethodDefinition("SetStage", Variant::StoredType::Void), &Shader::SetStage);
	type_registry::class_expose_method("Shader", ObjectRTTIModel::ObjectMethodDefinition("GetStage", Variant::StoredType::Int), &Shader::GetStage);
	
	ObjectRTTIModel::ObjectPropertyDefinition langProp = ObjectRTTIModel::ObjectPropertyDefinition("Language", Variant::StoredType::Int, false, "GetLanguage", "SetLanguage");
	ObjectRTTIModel::ObjectPropertyDefinition stageProp = ObjectRTTIModel::ObjectPropertyDefinition("Stage", Variant::StoredType::Int, false, "GetStage", "SetStage");
	type_registry::class_define_property("Shader", langProp);
	type_registry::class_define_property("Shader", stageProp);
}

vector<uint32_t> resources::Shader::CompileGLSLtoSPIRV(const std::string& source, ShaderResourceOptions::ShaderLanguage lang, ShaderResourceOptions::ShaderStage type)
{

	std::vector<uint32_t> spirv;
	std::string	info_log;
	glslang::InitializeProcess();

	auto messages = static_cast<EShMessages>(EShMsgReadHlsl | EShMsgDefault | EShMsgVulkanRules | EShMsgSpvRules);

	EShLanguage stage{};

	if (type == ShaderResourceOptions::ShaderStage::StageVert) stage = EShLangVertex;
	if (type == ShaderResourceOptions::ShaderStage::StageFrag) stage = EShLangFragment;
	if (type == ShaderResourceOptions::ShaderStage::StageTessControl) stage = EShLangTessControl;
	if (type == ShaderResourceOptions::ShaderStage::StageTessEval) stage = EShLangTessEvaluation;
	if (type == ShaderResourceOptions::ShaderStage::StageGeom) stage = EShLangGeometry;
	if (type == ShaderResourceOptions::ShaderStage::StageComp) stage = EShLangCompute;

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
