#pragma once

#include <chrono>

namespace chr = std::chrono;

chr::time_point<chr::system_clock, chr::seconds> DateTimeFromInteger(uint64_t millis);
uint64_t CreateRoom();
bool ConnectToRoom(uint64_t roomID);
