#include <iostream>
#include "core/init.h"
#include "tools/suids.h"

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

    return 0;
}