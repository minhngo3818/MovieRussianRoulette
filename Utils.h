// 53616c7465645f5f4ad3fc5e8fcde55c182286f0a530a2e3d25585c79a9d2887d159d8fff2b6a43ac9bab9a76c5729e0

#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <map>
#include <chrono>

using TimePoint = std::chrono::system_clock::time_point;

std::string formatTimePointToUtcTime(TimePoint timePoint);
std::string getCurrentTime1DayThreshold();
std::string getCurrentTimestamp();

#endif // UTILS_H