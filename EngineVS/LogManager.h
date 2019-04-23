#pragma once
#include "ThirdParty\spdlog\spdlog.h"
#include "ThirdParty\spdlog\sinks\basic_file_sink.h"

class LogManager
{
public:

	LogManager() {}
	virtual ~LogManager() {}
	
	virtual void VInitLogging();

private:
	std::shared_ptr<spdlog::logger> m_fileLogger;
};