#ifndef _INC_PD_CONFIG_HPP_
#define _INC_PD_CONFIG_HPP_
#include <pd/pd.hpp>

namespace pd {

    namespace config {
        void load();

        struct _world {
            float gravity_acceleration;
            float max_fall_speed;
        };

        struct _player {
            float max_movement_speed;
            float max_kinetic_movement_speed;
            float movement_acceleration;
            float kinetic_movement_acceleration;
            float friction;
            float max_jump_time;
            float jump_launch_velocity;
            float jump_control_power;
        };

        extern _world world;
        extern _player player;
    }
}

#endif
