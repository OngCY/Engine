#include "LogManager.h"

void LogManager::VInitLogging()
{
	//each log file is up to 5mb and overwrites after 10 files
	m_fileLogger = spdlog::rotating_logger_mt("rotating_logger", "logs\\gameLog.txt", 1048576 * 5, 10);
	spdlog::set_default_logger(m_fileLogger);
}

void LogManager::VCloseLogging()
{
	spdlog::drop_all();
}

std::shared_ptr<spdlog::logger> LogManager::VGetLogger()
{
	return m_fileLogger;
}