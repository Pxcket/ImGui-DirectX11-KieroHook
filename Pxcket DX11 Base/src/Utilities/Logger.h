#pragma once
#include <filesystem>
#include <iostream>
#include <Windows.h>
#include <mutex>
extern std::mutex logMutex;

namespace CMD
{
	void INIT_CMD();
	void write_log(const std::string& prefix, const std::string& message);
	void debug(const std::string& message);
	void success(const std::string& message);
	void error(const std::string& message);
	void log(const std::string& message);
}