#include <iostream>
#include "core/init.h"
#include "modules/executils.h"
#include "tools/suids.h"
#include "tools/web.h"

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


int main() {

    Init::welcome();
    Init::checkDependencies();

    auto suidFiles = findSUIDs();  // uses default "/"

    std::cout << RED << "\n================= SUID Files =================\n" << RESET;
    for (const auto& file : suidFiles) {
        std::cout << YELLOW << "  " << file << "\n";
    }
    std::cout << RED << "\n===============================================\n" << RESET;
    std::cout << "\n";


    std::cout << RED << "\n================= Download Binaries ===========\n\n" << RESET;
    if (commandExists("nmap") == -1){
        std::cout << RED << "Nmap is not installed, Downloading nmap binary" << RESET << "\n\n";
        Web("https://github.com/andrew-d/static-binaries/blob/master/binaries/linux/x86_64/nmap", "nmap");
    } else {
        std::cout << "Nmap already installed" << "\n";
    }
    if (commandExists("nc") == -1) {
        std::cout << RED << "Ncat not installed, Downloading netcat binary" << RESET << "\n\n";
        Web("https://github.com/andrew-d/static-binaries/blob/master/binaries/linux/x86_64/ncat", "nc");
    } else {
        std::cout << "Netcat already installed" << "\n";
    }
    std::cout << RED << "=================================================\n\n" << RESET;

    return 0;
}