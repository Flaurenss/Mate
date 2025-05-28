#include "Logger.h"
#include <iostream>

void Logger::Log(const std::string& message)
{
	std::cout << message << std::endl;
}

void Logger::War(const std::string& message)
{
	std::cout << message << std::endl;
}

void Logger::Err(const std::string& message)
{
	std::cerr << message << std::endl;
}
