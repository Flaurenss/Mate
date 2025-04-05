#pragma once
#include <vector>
#include <string>

enum LogType {
    INFO,
    WARNING,
    ERROR
};

class Logger
{
public:
    static void Log(const std::string& message);
    static void Err(const std::string& message);
};

