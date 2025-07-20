#pragma once
#include <string>

int commandExists(const std::string& cmd);
std::string execCommand(const std::string& cmd);