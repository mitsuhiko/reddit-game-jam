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

void pd::init_clock()
{
#if PD_PLATFORM == PD_PLATFORM_WINDOWS
    ULONG_PTR affinity_mask;
    ULONG_PTR process_affinity_mask;
    ULONG_PTR system_affinity_mask;

    if (!GetProcessAffinityMask(GetCurrentProcess(),
                                &process_affinity_mask,
                                &system_affinity_mask))
        return;

    // run on the first core
    affinity_mask = (ULONG_PTR)1 << 0;
    if (affinity_mask & process_affinity_mask)
        SetThreadAffinityMask(GetCurrentThread(), affinity_mask);
#endif
}

pd::timedelta_t pd::get_tick_frequency()
{
    return (pd::timedelta_t)SDL_GetPerformanceFrequency();
}

uint64_t pd::get_ticks()
{
    return (uint64_t)SDL_GetPerformanceCounter();
}

void pd::delay(uint64_t delay)
{
    SDL_Delay((uint32_t)delay);
}
