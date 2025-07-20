#include "web.h"
#include "executils.h"
#include <fstream>
#include <iostream>
#include <filesystem>
#include <cstdlib>   // for system()

// Optional ANSI color definitions if needed
#define RED "\033[31m"
#define RESET "\033[0m"



std::vector<unsigned char> Web(const std::string& url, const std::string& filename) {
    std::vector<unsigned char> results;

    try {
        if (commandExists("wget") == 1) {
            std::string cmd = "wget -q -O \"" + filename + "\" \"" + url + "\"";
            execCommand(cmd);
        }

        if (!std::filesystem::exists(filename)) {
            throw std::runtime_error("No file downloaded");
        }

        std::ifstream file(filename, std::ios::binary);
        if (!file) {
            throw std::runtime_error("Failed to open downloaded file");
        }

        results.assign(std::istreambuf_iterator<char>(file),
                       std::istreambuf_iterator<char>());
    }
    catch (const std::exception& e) {
        std::cerr << RED << "Error: " << e.what() << RESET << "\n";
    }

    return results;
}
