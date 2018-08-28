#pragma once
//Connecting the required libraries to boost.log.
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/support/date_time.hpp>

namespace logging = boost::log;
namespace src = boost::log::sources;
namespace expr = boost::log::expressions;
namespace keywords = boost::log::keywords;
using namespace logging::trivial;

void Logger_Initialization()
{
	src::severity_logger< severity_level > lg;
	logging::add_common_attributes();

	logging::add_file_log
	(
		//Set the log file.
		keywords::file_name = "sample_%N.log",
		//Set the logging format type:
		//¹: YYYY-MM-DD HH:MI:SS: <type> message.
		keywords::format =
		(
			expr::stream
			<< expr::attr< unsigned int >("LineID") << ": "
			<< expr::format_date_time< boost::posix_time::ptime >("TimeStamp", "%Y-%m-%d %H:%M:%S")
			<< ": <" << logging::trivial::severity
			<< "> " << expr::smessage
			)
	);
}

//Logging function.
void LOG(severity_level level, const std::string& message)
{
	src::severity_logger< severity_level > lg;
	BOOST_LOG_SEV(lg, level) << message;
	std::cout << "LOG: " << message << std::endl;
}

