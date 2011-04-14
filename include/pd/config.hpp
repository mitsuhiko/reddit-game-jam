#ifndef _INC_PD_CONFIG_HPP_
#define _INC_PD_CONFIG_HPP_
#include <pd/pd.hpp>
#include <pd/unordered_map.hpp>

namespace pd {

    namespace config {
        void load();

        struct animation_config {
            std::string texture;
            int frames;
            float speed;
        };

        struct weapon_config {
            animation_config anim;
            pd::vec2 offset;
            float width;
            float height;
        };

        struct block_def {
            int code;
            pd::unordered_map<int, animation_config> transitions;
        };

        struct _world {
            float gravity_acceleration;
            float max_fall_speed;
        };

        struct _camera {
            float adjust_time;
        };

        struct _console {
            int max_lines;
            bool print_to_stderr;
        };

        struct _blocks {
            pd::unordered_map<std::string, int> aliases;
            pd::unordered_map<int, block_def> defs;
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

            pd::vec2 thermal_offset;
            pd::vec2 kinetic_offset;
            pd::vec2 electromagnetic_offset;

            weapon_config flamethrower;
            weapon_config ice_spray;
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
        extern _console console;
        extern _blocks blocks;

        extern _player player;
        extern _kinetic_enemy kinetic_enemy;
    }
}

#endif
