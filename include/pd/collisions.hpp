#ifndef _INC_PD_COLLISIONS_HPP_
#define _INC_PD_COLLISIONS_HPP_
#include <pd/pd.hpp>


namespace pd {

    enum collision_report_flag {
        collided_top = 1,
        collided_right = 2,
        collided_bottom = 4,
        collided_left = 8
    };

    enum tile_collision_flag {
        passable,
        impassable,
        semi_passable
    };

    inline bool collided_horizontally(int mask)
    {
        return (mask & collided_left) != 0 || (mask & collided_right) != 0;
    }

    inline bool collided_vertically(int mask)
    {
        return (mask & collided_top) != 0 || (mask & collided_bottom) != 0;
    }
}

#endif
