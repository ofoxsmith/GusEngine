#include "file_helpers.h"
#include "core/globals.h"
#include <fstream>

std::vector<char> file_helpers::read_file(const std::string& filename) {
    std::ifstream file(filename, std::ios::ate | std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("failed to open file!");
    }
    size_t fileSize = (size_t)file.tellg();
    std::vector<char> buffer(fileSize);
    file.seekg(0);
    file.read(buffer.data(), fileSize);
    file.close();

    return buffer;
}

bool file_helpers::file_exists(const std::string& filename) {
    return fs::exists(filename);
}

vector<fs::path> file_helpers::get_files_in_dir(const string& dir)
{
    vector<fs::path> files;
    for (const auto& entry : fs::directory_iterator(dir)) {
        files.push_back(entry.path());
    }
    return files;
}
