#ifndef _INC_PD_MATH_HPP_
#define _INC_PD_MATH_HPP_
#include <pd/pd.hpp>

namespace pd {

    // Pixels to meters translation.
    template <class T>
    T pixel_to_meter(T a)
    {
        return a / 30;
    }

    template <class T>
    T meter_to_pixel(T a)
    {
        return a * 30;
    }

    template <class T>
    bool is_power_of_two(T value)
    {
        return (value & (value - 1)) == 0;
    }

    template <class T>
    T next_power_of_two(T value)
    {
        if (is_power_of_two(value))
            return value;
        value -= 1;
        for (size_t i = 1; i < sizeof(T) * 8; i <<= 1)
            value = value | value >> i;
        return value + 1;
    }
}

#endif
