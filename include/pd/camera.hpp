#ifndef _INC_PD_CAMERA_HPP_
#define _INC_PD_CAMERA_HPP_
#include <pd/pd.hpp>

namespace pd {

    class camera {
    public:
        camera();
        void look_at(const glm::vec2 &pos, pd::timedelta_t dt);
        void apply();

    private:
        glm::vec2 m_pos;
    };
}

#endif