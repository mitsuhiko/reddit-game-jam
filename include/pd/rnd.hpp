#ifndef _INC_PD_RND_HPP_
#define _INC_PD_RND_HPP_
#include <pd/pd.hpp>

namespace pd {

    class rnd {
    public:
        rnd();
        rnd(uint32_t seed);
        ~rnd();
        void seed(uint32_t seed);

        uint32_t next();
        double next_double();
        float next_float();

        template <class T>
        T range(T min, T max)
        {
            assert(min <= max);
            T width = max - min;
            return static_cast<T>(next_double() * width + min);
        }

        template <class T>
        T range(T max)
        {
            return static_cast<T>(next_double() * max);
        }

    private:
        rnd(const pd::rnd &);
        pd::rnd &operator=(const pd::rnd &);

        void *m_internal_rnd;
    };

    pd::rnd &default_rnd();
}

#endif
