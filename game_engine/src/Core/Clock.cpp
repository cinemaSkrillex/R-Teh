#include "../include/Core/ClockEngine.hpp"

namespace RealEngine {
Clock::Clock() {}

Clock::~Clock() {}

void Clock::restart() { _clock.restart(); }

}  // namespace RealEngine