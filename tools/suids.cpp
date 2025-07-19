#include "suids.h" 
#include <filesystem>
#include <vector>
#include <string>
#include <iostream>
#include <sys/stat.h>  // For checking permissions
#include <unistd.h>    // For access()
#include <pwd.h>       // For user info
#include <grp.h>       // For group info

std::vector<std::string> findSUIDs(const std::string path) {
    std::vector<std::string> suidFiles;

    try {
        for (const auto& entry : std::filesystem::recursive_directory_iterator(path, std::filesystem::directory_options::skip_permission_denied)) {
            if (!std::filesystem::is_regular_file(entry.path())) continue;

            struct stat fileStat;
            if (stat(entry.path().c_str(), &fileStat) == 0) {
                if (fileStat.st_mode & S_ISUID) {
                    suidFiles.push_back(entry.path().string());
                }
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Error scanning SUID files: " << e.what() << "\n";
    }

    return suidFiles;
}