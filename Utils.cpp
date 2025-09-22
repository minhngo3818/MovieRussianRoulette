// 53616c7465645f5f4ad3fc5e8fcde55c182286f0a530a2e3d25585c79a9d2887d159d8fff2b6a43ac9bab9a76c5729e0

#include "Utils.h"
#include <fstream>
#include <iostream>
#include <filesystem>


std::string formatTimePointToUtcTime(TimePoint timePoint) {
	std::time_t time = std::chrono::system_clock::to_time_t(timePoint);
	std::stringstream ss;
	ss << std::put_time(gmtime(&time), "%Y-%m-%dT%H:%M:%SZ"); // UTC format
	return ss.str();
}

std::string getCurrentTime1DayThreshold() {
	auto now = std::chrono::system_clock::now();
	auto past = now - std::chrono::hours(24);
	return formatTimePointToUtcTime(past);
}

std::string getCurrentTimestamp() {
	auto now = std::chrono::system_clock::now();
	return formatTimePointToUtcTime(now);
}