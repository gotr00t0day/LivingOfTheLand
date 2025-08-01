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

std::vector<std::string> wwwDir() {
    std::vector<std::string> content;
    std::vector<std::string> wwwPaths = {"/var/", "/usr/", "/opt/"};
    std::error_code ec;
    
    for (const auto& basePath : wwwPaths) {
        try {
            for (auto it = fs::recursive_directory_iterator(basePath, fs::directory_options::skip_permission_denied, ec);
                 it != fs::recursive_directory_iterator(); it.increment(ec)) {
                if (ec) {
                    ec.clear();
                    continue;
                }
                
                try {
                    if (it->is_directory()) {
                        std::string filename = it->path().filename().string();
                        std::string fullpath = it->path().string();
                        
                        if (filename == "www") {
                            content.push_back(fullpath);
                        }
                    }
                } catch (const fs::filesystem_error& e) {
                    // Skip individual file/directory errors
                    continue;
                }
            }
        } catch (const fs::filesystem_error& e) {
            // Skip if we can't access the base directory
            continue;
        }
    }
    return content;
}