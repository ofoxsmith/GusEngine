#include "resource_loader.h"
#include "file_helpers.h"
#include <shaderc/shaderc.hpp>

static std::string compile_file_to_assembly(const std::string& source_name, string type, const std::string& source) {
	shaderc::Compiler compiler;
	shaderc::CompileOptions options;
	shaderc_shader_kind kind(shaderc_glsl_default_vertex_shader);
	if (type == "vert") kind = shaderc_vertex_shader;
	if (type == "frag") kind = shaderc_fragment_shader;
	if (type == "tesc") kind = shaderc_tess_control_shader;
	if (type == "tese") kind = shaderc_tess_evaluation_shader;
	if (type == "geom") kind = shaderc_geometry_shader;
	if (type == "comp") kind = shaderc_compute_shader;
	auto name = file_helpers::get_file_name(source_name);

	options.SetGenerateDebugInfo();
	shaderc::AssemblyCompilationResult result = compiler.CompileGlslToSpvAssembly(
		source, kind, name.c_str(), options);

	if (result.GetCompilationStatus() != shaderc_compilation_status_success) {
		std::cerr << result.GetErrorMessage();
		return "";
	}

	return { result.cbegin(), result.cend() };
}

Shader* ResourceLoader::loadShaderFile(const string filePath, const string type)
{
	if (file_helpers::get_file_type(filePath) == "spv") {
		Shader* loaded = new Shader(file_helpers::read_file_binary(filePath), filePath);
		return loaded;
	}
	else {
		//Compile the shader using shaderc

		string data = file_helpers::read_file_text(filePath);

		string output = compile_file_to_assembly(filePath, type, data);
		std::vector<char> outputvec(output.begin(), output.end());

		Shader* loaded = new Shader(outputvec, filePath);
		return loaded;
	}
}

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

	if (resourceType == "glsl" || resourceType == "vert" || resourceType == "frag" || resourceType == "tesc" || resourceType == "tese" || resourceType == "geom" || resourceType == "comp" || resourceType == "spv") {
		return loadShaderFile(filePath, resourceType);
	}

	Log.Error("ResourceLoader", filePath + " is not a valid resource type.");
	return nullptr;
}