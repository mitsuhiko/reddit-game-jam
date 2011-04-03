#ifndef _INC_PD_CONFIG_HPP_
#define _INC_PD_CONFIG_HPP_
#include <pd/pd.hpp>

namespace pd {

    namespace config {
        void load();

        struct animation_config {
            std::string texture;
            int frames;
            float speed;
        };

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

            animation_config thermal_idle;
            animation_config kinetic_idle;
            animation_config electromagnetic_idle;
            animation_config flamethrower;
        };

        extern _world world;
        extern _player player;
    }
}

#endif
