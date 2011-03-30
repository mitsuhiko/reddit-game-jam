#ifndef _INC_PD_AABB_HPP_
#define _INC_PD_AABB_HPP_
#include <pd/pd.hpp>

namespace pd {

    struct aabb {
        glm::vec2 v1;
        glm::vec2 v2;

        aabb(const glm::vec2 &v1, const glm::vec2 &v2) : v1(v1), v2(v2)
        {
        }

        static pd::aabb make_box(const glm::vec2 &point,
                                 float width, float height)
        {
            pd::aabb rv(point, point + glm::vec2(width, height));
            if (rv.v1.x > rv.v2.x)
                std::swap(rv.v1.x, rv.v2.x);
            if (rv.v1.y > rv.v2.y)
                std::swap(rv.v1.y, rv.v2.y);
            return rv;
        }

        float left() const { return v1.x; }
        float top() const { return v1.y; }
        float right() const { return v2.x; }
        float bottom() const { return v2.y; }
        float width() const { return right() - left(); }
        float height() const { return bottom() - top(); }

        glm::vec2 center() const
        {
            return glm::vec2(left() + width() / 2.0f,
                             top() + height() / 2.0f);
        }

        glm::vec2 center_top() const
        {
            return glm::vec2(left() + width() / 2.0f, top());
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

        glm::vec2 intersection_depth(const aabb &other) const
        {
            float hw1 = width() / 2.0f;
            float hh1 = height() / 2.0f;
            float hw2 = other.width() / 2.0f;
            float hh2 = other.height() / 2.0f;

            glm::vec2 center1(v1 + glm::vec2(hw1, hh1));
            glm::vec2 center2(other.v1 + glm::vec2(hw2, hh2));

            float distx = center1.x - center2.x;
            float disty = center1.y - center2.y;
            float min_distx = hw1 + hw2;
            float min_disty = hh1 + hh2;

            if (std::abs(distx) >= min_distx ||
                std::abs(disty) >= min_disty)
                return glm::vec2();

            return glm::vec2(
                distx > 0.0f ? min_distx - distx : -min_distx - distx,
                disty > 0.0f ? min_disty - disty : -min_disty - disty
            );
        }
    };
}

#endif
