#include <filesystem>
#include <string>
#include <vector>

namespace fs = std::filesystem;

std::vector<std::string> dirRecursive() {
    std::vector<std::string> content;
    std::string currentPath = fs::current_path();
    for (const fs::directory_entry& dir_entry : fs::recursive_directory_iterator(currentPath, fs::directory_options::skip_permission_denied)) {
        content.push_back(dir_entry.path().string());
    }
    return content;
}