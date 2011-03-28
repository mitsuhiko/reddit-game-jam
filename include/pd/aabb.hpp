#ifndef _INC_PD_AABB_HPP_
#define _INC_PD_AABB_HPP_
#include <pd/pd.hpp>
#include <sstream>

namespace pd {

    struct aabb {
        glm::vec2 v1;
        glm::vec2 v2;

        aabb(const glm::vec2 &v1, const glm::vec2 &v2) : v1(v1), v2(v2)
        {
        }

        bool intersects(const aabb &other) const
        {
            return !(
                v1.y > other.v2.y ||
                v2.y < other.v1.y ||
                v1.x > other.v2.x ||
                v2.x < other.v1.x
            );
        }
    };
}

#endif
