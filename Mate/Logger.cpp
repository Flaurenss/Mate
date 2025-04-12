#include "Logger.h"
#include <iostream>

void Logger::Log(const std::string& message)
{
	std::cout << "\x1B[32m" << message << "\033[0m" << std::endl;
}

void Logger::War(const std::string& message)
{
	std::cout << "\x1B[93m" << message << "\033[0m" << std::endl;
}

void Logger::Err(const std::string& message)
{
	std::cerr << "\x1B[91m" << message << "\033[0m" << std::endl;
}
