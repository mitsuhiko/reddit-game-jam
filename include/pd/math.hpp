#ifndef _INC_PD_MATH_HPP_
#define _INC_PD_MATH_HPP_
#include <pd/pd.hpp>
#include <pd/config.hpp>
#include <limits>


namespace pd {

    static const double pi = 3.14159265358979323846;

    template <class T>
    T sign(T val)
    {
        if (val > 0)
            return T(1);
        else if (val == 0)
            return T(0);
        return T(-1);
    }

    template <class T>
    T clamp(T val, T min, T max)
    {
        if (val < min)
            return min;
        if (val > max)
            return max;
        return val;
    }

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
    T deg_to_rad(T value)
    {
        return (T)(value * pi / 180.0);
    }

    template <class T>
    T rad_to_deg(T value)
    {
        return (T)(value * 180.0 / pi);
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

    template <class T>
    bool almost_equal(T a, T b)
    {
        static const T e = std::numeric_limits<T>::epsilon() / T(2);
        return (a - e < b && a + e > b);
    }

    template <class T, class F>
    T lerp(T a, T b, F factor)
    {
        return a + (b - a) * pd::clamp(factor, F(0), F(1));
    }

    /* applies gravitiy on the velocity vector */
    inline glm::vec2 apply_gravity(const glm::vec2 &vel, float dt)
    {
        return glm::vec2(
            vel.x,
            pd::clamp(vel.y + pd::gravity_acceleration * dt,
                      -pd::max_fall_speed, pd::max_fall_speed)
        );
    }
}

#endif
