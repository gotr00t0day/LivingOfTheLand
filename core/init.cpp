#include "init.h"
#include "../modules/executils.h"
#include "../modules/parsers.h"
#include <string>
#include <exception>
#include <iostream>
#include <cstdlib>
#include <vector>

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"
#define BOLD    "\033[1m"
#define UNDERLINE "\033[4m"


void Init::welcome() {
    std::cout << BOLD << R"(

 ██▓     ▒█████  ▄▄▄█████▓ ██▓    
▓██▒    ▒██▒  ██▒▓  ██▒ ▓▒▓██▒    
▒██░    ▒██░  ██▒▒ ▓██░ ▒░▒██░    
▒██░    ▒██   ██░░ ▓██▓ ░ ▒██░    
░██████▒░ ████▓▒░  ▒██▒ ░ ░██████▒ 
░ ▒░▓  ░░ ▒░▒░▒░   ▒ ░░   ░ ▒░▓  ░
░ ░ ▒  ░  ░ ▒ ▒░     ░    ░ ░ ▒  ░
  ░ ░   ░ ░ ░ ▒    ░        ░ ░   
    ░  ░    ░ ░               ░  ░

    Living Of The Land Toolkit


Author:  c0d3Ninja
Version: v0.11

========================================================
    )" << RESET << std::endl;
}


// Config Struct for all the vectors dealing with the config config/lotl.conf file
// Output as well
struct Config{
    std::vector<std::string> required;
    std::vector<std::string> optional;
    std::vector<std::string> credPaths;
    std::vector<std::string> bashHistory;
    std::vector<std::string> requiredCommands;
    std::vector<std::string> optionalCommands;
    std::vector<std::string> credPathsFound;
    std::vector<std::string> bashHistoryFound;
    std::vector<std::string> logs;
    std::vector<std::string> logsFound;
    std::vector<std::string> checkSSH;
    std::vector<std::string> sshFound;
    std::vector<std::string> checkMount;
    std::vector<std::string> mountFound;
    std::vector<std::string> backUP;
    std::vector<std::string> backupFound;
    std::vector<std::string> sensitive;
    std::vector<std::string> sensitiveFound;
};

void Init::checkDependencies() {

    Config ParseConf;
    Config OutputConf;

    ParseConf.required = parseDependencies("config/lotl.conf", "required");
    ParseConf.optional = parseDependencies("config/lotl.conf", "optional");
    ParseConf.credPaths = parseDependencies("config/lotl.conf", "checkCreds");
    ParseConf.bashHistory = parsePaths("config/lotl.conf", "checkBashHistory");
    ParseConf.logs = parsePaths("config/lotl.conf", "checkLogs");
    ParseConf.checkSSH = parsePaths("config/lotl.conf", "checkSSH");
    ParseConf.backUP = parsePaths("config/lotl.conf", "CheckBackUp");
    ParseConf.sensitive = parsePaths("config/lotl.conf", "checkSensitive");

    for (const auto& path : ParseConf.credPaths) {
        if (checkPaths(path) == 1) {
            OutputConf.credPathsFound.push_back(path);
        }
    }

    for (const auto& path : ParseConf.bashHistory) {
        std::string expanded = expandPath(path);
        if (checkPaths(expanded) == 1) {
            OutputConf.bashHistoryFound.push_back(path);  // ✅ Correct vector
        }
    }

    for (const auto& cmd : ParseConf.required) {
        if (commandExists(cmd)) {
            OutputConf.requiredCommands.push_back(cmd);
        }
    }
    for (const auto& cmd: ParseConf.optional) {
        if(commandExists(cmd)) {
            OutputConf.optionalCommands.push_back(cmd);
        }
    }

    for (const auto& path : ParseConf.logs) {
        std::string expanded = expandPath(path);
        if (checkPaths(expanded) == 1) {
            OutputConf.logsFound.push_back(path);  // ✅ Correct vector
        }
    }

    for (const auto& path : ParseConf.checkSSH) {
        std::string expanded = expandPath(path);
        if (checkPaths(expanded) == 1) {
            OutputConf.sshFound.push_back(path);  // ✅ Correct vector
        }
    }

    for (const auto& path : ParseConf.checkMount) {
        std::string expanded = expandPath(path);
        if (checkPaths(expanded) == 1) {
            OutputConf.mountFound.push_back(path);  // ✅ Correct vector
        }
    }

    for (const auto& path : ParseConf.backUP) {
        std::string expanded = expandPath(path);
        if (checkPaths(expanded) == 1) {
            OutputConf.backupFound.push_back(path);  // ✅ Correct vector
        }
    }

    for (const auto& path : ParseConf.sensitive) {
        std::string expanded = expandPath(path);
        if (checkPaths(expanded) == 1) {
            OutputConf.sensitiveFound.push_back(path);  // ✅ Correct vector
        }
    }

    std::cout << "[*] Checking dependencies..." << "\n\n";

    std::cout << GREEN << "[+] Found: " << CYAN << OutputConf.requiredCommands.size() << RESET << " " << "(required) dependencies" << "\n";
    for (std::string output : OutputConf.requiredCommands) {
        std::cout << "\t" << YELLOW << output << RESET << "\n";
    }
    std::cout << GREEN << "[+] Found: " << CYAN << OutputConf.optionalCommands.size() << RESET <<  " " << "(optional) dependencies" << "\n";
    for (std::string output : OutputConf.optionalCommands) {
        std::cout << "\t" << YELLOW << output << RESET << "\n";
    }
    std::cout << GREEN << "[+] Found: " << CYAN << OutputConf.credPathsFound.size() << RESET << " " << "(Paths that might contain creds)" << "\n";
    for (std::string output : OutputConf.credPathsFound) {
        std::cout << "\t" << YELLOW << output << RESET << "\n";
    }
    std::cout << GREEN << "[+] Found: " << CYAN << OutputConf.bashHistoryFound.size() << RESET << " " << "History files" << "\n";
    for (std::string output : OutputConf.bashHistoryFound) {
        std::cout << "\t" << YELLOW << output << RESET << "\n";
    }
    std::cout << GREEN << "[+] Found: " << CYAN << OutputConf.logsFound.size() << RESET << " " << "Log files" << "\n";
    for (std::string output : OutputConf.logsFound) {
        std::cout << "\t" << YELLOW << output << RESET << "\n";
    }
    std::cout << GREEN << "[+] Found: " << CYAN << OutputConf.sshFound.size() << RESET << " " << "SSH files" << "\n";
    for (std::string output : OutputConf.sshFound) {
        std::cout << "\t" << YELLOW << output << RESET << "\n";
    }
    std::cout << GREEN << "[+] Found: " << CYAN << OutputConf.backupFound.size() << RESET << " " << "Backup files" << "\n";
    for (std::string output : OutputConf.backupFound) {
        std::cout << "\t" << YELLOW << output << RESET << "\n";
    }
    std::cout << GREEN << "[+] Found: " << CYAN << OutputConf.sensitiveFound.size() << RESET << " " << "Sensitive files" << "\n";
    for (std::string output : OutputConf.sensitiveFound) {
        std::cout << "\t" << YELLOW << output << RESET << "\n";
    }

    // Get system information
    std::string unameResults;
    std::string hostnameResults;
    std::string whoamiResults;
    std::string uptimeResults;
    std::string mountResults;
    
    try {
        for (const std::string& cmd : OutputConf.requiredCommands) {
            if (cmd == "uname") {
                unameResults = execCommand("uname -a");
            }
            else if (cmd == "hostname") {
                hostnameResults = execCommand("hostname");
            } else if (cmd == "whoami"){
                uptimeResults = execCommand("uptime");
            } else if (cmd == "mount") {
                mountResults = execCommand("mount");
            }
        }
        // Optional: Trim newlines
        if (!unameResults.empty()) {
            unameResults.erase(unameResults.find_last_not_of("\n") + 1);
        }
        if (!hostnameResults.empty()) {
            hostnameResults.erase(hostnameResults.find_last_not_of("\n") + 1);
        }
        if (!uptimeResults.empty()) {
            uptimeResults.erase(uptimeResults.find_last_not_of("\n") + 1);
        }
        if (!mountResults.empty()) {
            mountResults.erase(mountResults.find_last_not_of("\n") + 1);
        }
    }
    catch (const std::exception& e) {
        std::cerr <<  "Error: " << e.what() << "\n";
    }

    // ifconfig
    std::string ifconfig;
    std::string inet;
    if (commandExists("ifconfig")) {
        ifconfig = execCommand("ifconfig");
        if (!ifconfig.empty()) {
            ifconfig.erase(ifconfig.find_last_not_of("\n") + 1);
            size_t pos = ifconfig.find("inet ");
            if (pos != std::string::npos) {
                size_t start = pos + 5; // Skip "inet "
                size_t end = ifconfig.find(" ", start);
                if (end != std::string::npos) {
                    std::string ip = ifconfig.substr(start, end - start);
                    inet = ip;
                }
            }
        }
    }

    std::cout << "\n";

    std::cout << RED << "\t================= System Information =================\n\n" << RESET;
    std::cout << "Uname: " << YELLOW << unameResults << RESET << "\n\n";
    std::cout << "Hostname: " << YELLOW << hostnameResults << RESET << "\n\n";
    std::cout << "Uptime: " << YELLOW << uptimeResults << RESET << "\n\n";
    std::cout << "Mount: \n" << YELLOW << mountResults << RESET << "\n\n";
    std::cout << "IP: " << YELLOW << inet << RESET << "\n\n";
    std::cout << RED << "\t======================================================\n\n" << RESET;


    std::cout << "\n";
    std::cout << RED << "Displaying the contents of the paths that might contain creds..." << "\n\n";

    for (const auto& path : OutputConf.credPathsFound) {
        std::string expanded = expandPath(path);
        if (checkPaths(expanded) == 1) {
            std::string cmd = "cat \"" + expanded + "\"";
            std::string output = execCommand(cmd);
            std::cout << GREEN << "[+] Contents of " << MAGENTA << path << ":\n" <<RESET << YELLOW << output << RESET << "\n";
        }
    }  
    std::cout << "\n";
    std::cout << RED << "Displaying the contents of the history files..." << "\n\n";

    for (const auto& path : OutputConf.bashHistoryFound) {
        std::string expanded = expandPath(path);
        if (checkPaths(expanded) == 1) {
            if (expanded == std::string(getenv("HOME")) + "/.zsh_history") {
                // Don't display the content of zsh_history
                continue;
            }
            std::string cmd = "cat \"" + expanded + "\"";
            std::string output = execCommand(cmd);
            std::cout << GREEN << "[+] Contents of " << MAGENTA << path << ":\n" << RESET << YELLOW << output << RESET << "\n";
        }
    }
    std::cout << "\n";
    if (OutputConf.sensitiveFound.size() > 0) {
        std::cout << RED << "Displaying the contents of the sensitive files..." << "\n\n";

        for (const auto& path : OutputConf.sensitiveFound) {
            std::string expanded = expandPath(path);
            if (checkPaths(expanded) == 1) {
                std::string cmd = "cat \"" + expanded + "\"";
                std::string output = execCommand(cmd);
                std::cout << GREEN << "[+] Contents of " << MAGENTA << path << ":\n" << RESET << YELLOW << output << RESET << "\n";
            }
        }
    }
    if (OutputConf.logsFound.size() > 0) {
        std::cout << "\n";
        std::cout << RED << "Displaying the contents of the logs..." << "\n\n";
        for (const auto& path : OutputConf.logsFound) {
            std::string expanded = expandPath(path);
            if (checkPaths(expanded) == 1) {
                std::string cmd = "cat \"" + expanded + "\"";
                std::string output = execCommand(cmd);
                std::cout << GREEN << "[+] Contents of " << MAGENTA << path << ":\n" << RESET << YELLOW << output << RESET << "\n";
            }
        }
    }

    if (OutputConf.sshFound.size() > 0) {
        std::cout << "\n";
        std::cout << RED << "Displaying the contents of the ssh files..." << "\n\n";
        for (const auto& path : OutputConf.sshFound) {
            std::string expanded = expandPath(path);
            if (checkPaths(expanded) == 1) {
                std::string cmd = "cat \"" + expanded + "\"";
                std::string output = execCommand(cmd);
                std::cout << GREEN << "[+] Contents of " << MAGENTA << path << ":\n" << RESET << YELLOW << output << RESET << "\n";
            }
        }
    }
}