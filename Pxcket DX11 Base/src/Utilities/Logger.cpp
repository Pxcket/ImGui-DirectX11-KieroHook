#include "Logger.h"
#include <chrono>
#include <fstream>
std::mutex logMutex;

void CMD::INIT_CMD()
{
	std::lock_guard<std::mutex> guard(logMutex);

	std::filesystem::path logPath("log.txt");
	std::filesystem::path oldLogPath("log_old.txt");

	if (std::filesystem::exists(oldLogPath)) { std::filesystem::remove(oldLogPath); }
	if (std::filesystem::exists(logPath)) { std::filesystem::rename(logPath, oldLogPath); }

}

void CMD::write_log(const std::string& prefix, const std::string& message)
{
	std::lock_guard<std::mutex> guard(logMutex);

	std::string formatted = "[ " + prefix + " ] " + message;

	std::cout << formatted << std::endl;

	std::ofstream logFile("log.txt", std::ios_base::app);
	if (logFile.is_open())
	{
		logFile << formatted << std::endl;
	}
}

void CMD::debug(const std::string& message)
{
#ifdef PPV
	CMD::write_log(">", message);
#endif
}

void CMD::success(const std::string& message)
{
#ifdef PPV
	CMD::write_log("+", message);
#endif
}

void CMD::error(const std::string& message)
{
#ifdef PPV
	CMD::write_log("-", message);
#endif
}

void CMD::log(const std::string& message)
{
#ifdef PPV
	CMD::write_log("*", message);
#endif
}