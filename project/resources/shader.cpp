#include "shader.h"
#include "filesystem/file_helpers.h"
resources::Shader::Shader(const string path): Resource("Shader", path) {
	compiledCode = file_helpers::read_file(path);
}
