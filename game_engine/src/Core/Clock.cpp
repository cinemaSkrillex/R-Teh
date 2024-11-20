#include "Clock.hpp"

namespace engine {
Clock::Clock() {}

Clock::~Clock() {}

void Clock::restart() { _clock.restart(); }

} // namespace engine