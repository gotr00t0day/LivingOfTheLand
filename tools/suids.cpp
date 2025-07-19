#include "suids.h"
#include <filesystem>
#include <iostream>
#include <sys/stat.h>
#include <unistd.h>

std::vector<std::string> findSUIDs(const std::string path) {
    std::vector<std::string> suidFiles;

    std::error_code ec;
    auto iter = std::filesystem::recursive_directory_iterator(
        path, std::filesystem::directory_options::skip_permission_denied, ec);
    
    if (ec) {
        std::cerr << "Error creating directory iterator: " << ec.message() << "\n";
        return suidFiles;
    }

    try {
        for (auto it = iter; it != std::filesystem::recursive_directory_iterator(); it.increment(ec)) {
            if (ec) {
                // Skip directories that cause permission errors and continue
                ec.clear();
                continue;
            }

            try {
                if (!std::filesystem::is_regular_file(it->path())) continue;
                if (access(it->path().c_str(), R_OK) != 0) continue;

                struct stat fileStat;
                if (stat(it->path().c_str(), &fileStat) == 0) {
                    if (fileStat.st_mode & S_ISUID) {
                        suidFiles.push_back(it->path().string());
                    }
                } else {
                    // Skip files that can't be stat'd (permission denied, etc.)
                    continue;
                }
            } catch (const std::filesystem::filesystem_error& fe) {
                // Skip individual files that cause filesystem errors
                continue;
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Error scanning SUID files: " << e.what() << "\n";
    }

    return suidFiles;
}