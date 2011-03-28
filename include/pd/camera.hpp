#ifndef _INC_PD_CAMERA_HPP_
#define _INC_PD_CAMERA_HPP_
#include <pd/pd.hpp>

namespace pd {

    class camera {
    public:
        camera();
        void look_at(float x, float y, pd::timedelta_t dt);
        void apply();

    private:
        float m_x;
        float m_y;
    };
}

#endif