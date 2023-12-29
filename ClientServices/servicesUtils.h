#pragma once

#include <chrono>
#include <cstdint>

namespace chr = std::chrono;

namespace utils
{
	chr::time_point<chr::system_clock, chr::seconds> DateTimeFromInteger(uint64_t millis);
	uint64_t NowAsInteger();
}
