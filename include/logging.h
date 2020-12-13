#ifndef LOGGING_H
#define LOGGING_H
/*#include <log4cpp/Category.hh>
#include <log4cpp/Appender.hh>
#include <log4cpp/OstreamAppender.hh>
#include <log4cpp/FileAppender.hh>
#include <log4cpp/Layout.hh>
#include <log4cpp/BasicLayout.hh>
#include <log4cpp/Priority.hh>
#include <log4cpp/NDC.hh>
#include <log4cpp/PropertyConfigurator.hh>*/
#include <iostream>
#include <string.h>

//#define CODE_LOCATION __FILE__
//#define FILE (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

// DEBUG < INFO < NOTICE < WARN < ERROR < CRIT < ALERT < FATAL = EMERG
#define EMERG(__logstream)  __logstream << std::endl << "[EMERG] "  //__logstream << log4cpp::Priority::EMERG << "[" << FILE << ":" << __LINE__ << ":" << __FUNCTION__ << "] - " 
#define ALERT(__logstream)  __logstream << std::endl << "[ALERT] "  //__logstream << log4cpp::Priority::ALERT << "[" << FILE << ":" << __LINE__ << ":" << __FUNCTION__ << "] - "
#define CRIT(__logstream)   __logstream << std::endl << "[CRIT] "  //__logstream << log4cpp::Priority::CRIT << "[" << FILE << ":" << __LINE__ << ":" << __FUNCTION__ << "] - "
#define ERROR(__logstream)  __logstream << std::endl << "[ERROR] "  //__logstream << log4cpp::Priority::ERROR << "[" << FILE << ":" << __LINE__ << ":" << __FUNCTION__ << "] - "
#define WARN(__logstream)   __logstream << std::endl << "[WARN] "  //__logstream << log4cpp::Priority::WARN << "[" << FILE << ":" << __LINE__ << ":" << __FUNCTION__ << "] - "
#define NOTICE(__logstream) __logstream << std::endl << "[NOTICE] "  //__logstream << log4cpp::Priority::NOTICE << "[" << FILE << ":" << __LINE__ << ":" << __FUNCTION__ << "] - "
#define INFO(__logstream)  __logstream << std::endl << "[INFO] "  //__logstream << log4cpp::Priority::INFO << "[" << FILE << ":" << __LINE__ << ":" << __FUNCTION__ << "] - "
#define DEBUG(__logstream) __logstream << std::endl << "[DEBUG] "  //__logstream << log4cpp::Priority::DEBUG << "[" << FILE << ":" << __LINE__ << ":" << __FUNCTION__ << "] - "

#define LOG std::cout
//log4cpp::Category &LOG = log4cpp::Category::getRoot();
#endif