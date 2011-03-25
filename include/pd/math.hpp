#ifndef _INC_PD_MATH_HPP_
#define _INC_PD_MATH_HPP_
#include <pd/pd.hpp>

namespace pd {

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
        for (int i = 1; i < sizeof(T) * 8; i <<= 1)
            value = value | value >> i;
        return value + 1;
    }
}

#endif
