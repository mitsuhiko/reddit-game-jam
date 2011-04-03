#include <pd/utils.hpp>
#include <time.h>


int64_t pd::seconds_since_epoch()
{
#if PD_PLATFORM == PD_PLATFORM_WINDOWS
    int64_t time;
    _time64(&time);
    return time;
#else
    time_t rv = time(0);
    return (int64_t)rv;
#endif
}

uint64_t pd::get_ticks()
{
    return SDL_GetTicks();
}

void pd::delay(uint64_t delay)
{
    SDL_Delay((uint32_t)delay);
}