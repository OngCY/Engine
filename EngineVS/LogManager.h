#pragma once
#include "ThirdParty\spdlog\spdlog.h"
#include "ThirdParty\spdlog\sinks\basic_file_sink.h"

class LogManager
{
public:

	LogManager() {}
	virtual ~LogManager() {}
	
	virtual void VInitLogging();
	virtual void VCloseLogging();
	virtual std::shared_ptr<spdlog::logger> VGetLogger();

private:
	std::shared_ptr<spdlog::logger> m_fileLogger;
};

class NullLogManager : public LogManager
{
public:
	NullLogManager() {}
	virtual ~NullLogManager() {}

	virtual void VInitLogging() {}
	virtual void VCloseLogging() {}
	virtual std::shared_ptr<spdlog::logger> VGetLogger() { return nullptr; }
};