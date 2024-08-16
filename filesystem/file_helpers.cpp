#include "file_helpers.h"
#include "core/globals.h"
#include <fstream>

inline void file_helpers::remove_path_prefix(string& path)
{
    if (!path.starts_with("://")) {
        Log.Warn("I/O", "Filepaths relative to project root should start with '://' Assuming " + path + " is relative to root.");
        return;
    }
    path.erase(0, 3);
    return;
}

std::vector<char> file_helpers::read_file(std::string filename) {

    Log.Debug("I/O", "Reading file " + filename);
    file_helpers::remove_path_prefix(filename);
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

bool file_helpers::file_exists(std::string filename) {
    file_helpers::remove_path_prefix(filename);
    return fs::exists(filename);
}

string file_helpers::get_file_type(string filename) {
    file_helpers::remove_path_prefix(filename);
    string extension = fs::path(filename).extension().string();
    if (extension.size() > 2) extension.erase(0, 1);
    return extension;
}

vector<fs::path> file_helpers::get_files_in_dir(string dir) {
    file_helpers::remove_path_prefix(dir);
    vector<fs::path> files;
    for (const auto& entry : fs::directory_iterator(dir)) {
        files.push_back(entry.path());
    }
    return files;
}
