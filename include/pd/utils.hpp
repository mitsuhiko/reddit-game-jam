#ifndef _INC_PD_UTILS_HPP_
#define _INC_PD_UTILS_HPP_
#include <pd/pd.hpp>

namespace pd {

    int64_t seconds_since_epoch();

    void init_clock();

    pd::timedelta_t get_tick_frequency();

    uint64_t get_ticks();

    void delay(uint64_t delay);
}

#endif
