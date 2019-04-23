#include "LogManager.h"

void LogManager::VInitLogging()
{
	m_fileLogger = spdlog::basic_logger_mt("basic_logger", "logs/basic.txt");
	spdlog::set_default_logger(m_fileLogger);
	spdlog::register_logger(m_fileLogger);
}