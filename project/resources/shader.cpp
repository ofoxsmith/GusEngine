#include "shader.h"
resources::Shader::Shader(std::vector<char> source, const string path): Resource("Shader", path) {
	compiledCode = source;
}
