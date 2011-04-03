#ifndef _INC_PD_CAMERA_HPP_
#define _INC_PD_CAMERA_HPP_
#include <pd/pd.hpp>
#include <pd/math.hpp>

namespace pd {

    class player;

    class camera {
    public:
        camera();
        void look_at(const pd::vec2 &pos, pd::timedelta_t dt);
        void apply();

    private:
        pd::vec2 m_pos;
        pd::vec2 m_velocity;
        pd::vec2 m_acceleration;
    };
}

#endif
