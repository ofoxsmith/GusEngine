#include "shader.h"
resources::Shader::Shader(vector<uint32_t> source, const string path, const string sourceFilepath): Resource("Shader", path, sourceFilepath) {
	compiledCode = source;
}
