#include <pd/utils.hpp>
#include <time.h>

#if PD_PLATFORM == PD_PLATFORM_WINDOWS
uint64_t win32_pc_start;
uint64_t win32_pc_freq;
bool win32_use_pc;
#endif


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

    LARGE_INTEGER freq, ctr;
    if (!QueryPerformanceFrequency(&freq) ||
        !QueryPerformanceCounter(&ctr)) {
        PD_LOG("Unable to use QueryPerformanceCounter");
        win32_use_pc = false;
        return;
    }

    win32_use_pc = true;
    win32_pc_freq = freq.QuadPart;
    win32_pc_start = ctr.QuadPart;
#endif
}

pd::timedelta_t pd::get_tick_frequency()
{
#if PD_PLATFORM == PD_PLATFORM_WINDOWS
    if (win32_use_pc)
        return (pd::timedelta_t)win32_pc_freq;
#endif
    return 1000.0f;
}

uint64_t pd::get_ticks()
{
#if PD_PLATFORM == PD_PLATFORM_WINDOWS
    if (win32_use_pc) {
        LARGE_INTEGER ctr;
        QueryPerformanceCounter(&ctr);
        return (uint64_t)(ctr.QuadPart - win32_pc_start);
    }
#endif
    return (uint64_t)SDL_GetTicks();
}

void pd::delay(uint64_t delay)
{
    SDL_Delay((uint32_t)delay);
}
