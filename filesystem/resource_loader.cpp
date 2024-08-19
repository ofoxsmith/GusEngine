#include "resource_loader.h"
#include "file_helpers.h"

#include <glslang/Public/ShaderLang.h>
#include <glslang/SPIRV/GlslangToSpv.h>
#include <glslang/Public/ResourceLimits.h>

Resource* ResourceLoader::_load(const string filePath) {
	if (!file_helpers::file_exists(filePath)) {
		Log.Error("ResourceLoader", filePath + " does not exist.");
		return nullptr;
	}

	string resourceType = file_helpers::get_file_type(filePath);
	if (resourceType == "res") {
		// The file is a standard resource, and can be converted directly to a resource
	}
	if (resourceType == "pres") {
		// The file is a property resource, and contains engine settings for an external data resource
	}

	// The file is an external data resource, and must be encapsulated in a property resource
	
	if (resourceType == "jpg" || resourceType == "jpeg" || resourceType == "png" || resourceType == "bmp" || resourceType == "psd" || resourceType == "tga"
		|| resourceType == "gif" || resourceType == "hdr" || resourceType == "pic" || resourceType == "ppm" || resourceType == "pgm") {
		

	}

	if (resourceType == "vert" || resourceType == "frag" || resourceType == "tesc" || resourceType == "tese" || resourceType == "geom" || resourceType == "comp" || resourceType == "spv") {
		return loadShaderFile(filePath, resourceType);
	}

	Log.Error("ResourceLoader", filePath + " is not a valid resource type.");
	return nullptr;
}

static vector<uint32_t> compileGLSLtoSPIRV(const std::string& source, string type)
{

	std::vector<uint32_t> spirv;
	std::string	info_log;
	glslang::InitializeProcess();

	auto messages = static_cast<EShMessages>(EShMsgReadHlsl | EShMsgDefault | EShMsgVulkanRules | EShMsgSpvRules);

	EShLanguage stage {};

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

Shader* ResourceLoader::loadShaderFile(const string filePath, const string type)
{
	if (file_helpers::get_file_type(filePath) == "spv") {
		auto spv = file_helpers::read_file_binary(filePath);
		Shader* loaded = new Shader(spv, "", filePath);
		return loaded;
	}
	else {

		string data = file_helpers::read_file_text(filePath);

		vector<uint32_t> output = compileGLSLtoSPIRV(data, file_helpers::get_file_type(filePath));

		Shader* loaded = new Shader(output, "", filePath);
		return loaded;
	}
}
