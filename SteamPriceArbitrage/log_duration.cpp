#include "log_duration.h"

LogDuration::LogDuration(const std::string& msg, std::ostream& ostr)
	: message(msg + ": ")
	, ostr(ostr)
	, start(std::chrono::steady_clock::now()) {}

LogDuration::~LogDuration() {

	auto finish = std::chrono::steady_clock::now();
	auto dur = finish - start;

	ostr << message
		<< std::chrono::duration_cast<std::chrono::milliseconds>(dur).count()
		<< " ms" << std::endl << std::endl;

}