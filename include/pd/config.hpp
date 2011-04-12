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

        struct _camera {
            float adjust_time;
        };

        struct _player {
            float width;
            float height;

            float max_movement_speed;
            float max_kinetic_movement_speed;
            float movement_acceleration;
            float kinetic_movement_acceleration;
            float friction;
            float air_friction;
            float jump_impulse;

            animation_config thermal_idle;
            animation_config kinetic_idle;
            animation_config electromagnetic_idle;
            animation_config flamethrower;

            pd::vec2 thermal_offset;
            pd::vec2 kinetic_offset;
            pd::vec2 electromagnetic_offset;
            pd::vec2 flamethrower_offset;
        };

        struct _kinetic_enemy {
            float width;
            float height;

            float spawn_fall_speed;
            float movement_speed;
            float dash_speed;
            float dash_countdown;
            float see_distance;

            animation_config walk;
            animation_config dash;

            pd::vec2 default_offset;
            pd::vec2 dash_offset;
        };

        extern _world world;
        extern _camera camera;
        extern _player player;
        extern _kinetic_enemy kinetic_enemy;
    }
}

#endif
