#include "bvs/logger.h"

#ifdef BVS_LOG_SYSTEM
#include "logsystem.h"
#endif



BVS::Logger::Logger(std::string name, unsigned short verbosity, LogTarget target)
	: verbosity(verbosity)
	, target(target)
	, name(name)
#ifdef BVS_LOG_SYSTEM
	, logSystem(LogSystem::connectToLogSystem())
#endif
{
#ifdef BVS_LOG_SYSTEM
	logSystem->announce(*this);
#endif
}



std::ostream& BVS::Logger::out(const int level)
{
#ifdef BVS_LOG_SYSTEM
	return logSystem->out(*this, level);
#else
	return std::cout;
#endif
}



void BVS::Logger::endl()
{
#ifdef BVS_LOG_SYSTEM
	logSystem->endl();
#endif
}



std::string BVS::Logger::getName() const
{
	return this->name;
}



BVS::Logger::~Logger()
{

}
