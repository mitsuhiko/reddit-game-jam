#ifndef _INC_PD_COLOR_HPP_
#define _INC_PD_COLOR_HPP_
#include <pd/pd.hpp>

namespace pd {

    struct color {
        typedef unsigned char component_t;

        component_t r;
        component_t g;
        component_t b;
        component_t a;

        color(uint32_t val)
        {
            r = val >> 24;
            g = (val >> 16) & 0xff;
            b = (val >> 8) & 0xff;
            a = val & 0xff;
        }

        color() : r(0), g(0), b(0), a(255)
        {
        }

        color(const std::string &value);

        void to_float(float &r_out, float &g_out, float &b_out, float &a_out) const
        {
            r_out = r / 255.0f;
            g_out = g / 255.0f;
            b_out = b / 255.0f;
            a_out = a / 255.0f;
        }

        void to_float(float *arr) const
        {
            to_float(arr[0], arr[1], arr[2], arr[3]);
        }
    };
}

#endif
