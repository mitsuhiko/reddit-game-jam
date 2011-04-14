#ifndef _INC_PD_PLAYER_HPP_
#define _INC_PD_PLAYER_HPP_
#include <pd/pd.hpp>
#include <pd/entity.hpp>
#include <pd/animation.hpp>

namespace pd {

    class block;
    class session;

    class player : public pd::entity {
    public:
        enum stance_type {
            kinetic_stance,
            thermal_stance,
            electromagnetic_stance
        };

        player(pd::game_session *session, const pd::vec2 &pos);

        float energy() const { return m_energy; }
        bool looks_right() const { return !m_flipped; }
        bool looks_left() const { return m_flipped; }

        stance_type stance() const { return m_stance; }
        void stance(stance_type val) { m_stance = val; }
        const pd::animation *current_animation() const;
        const pd::vec2 &bound_offset() const;
        
        void apply_physics(pd::timedelta_t dt);
        const pd::config::weapon_config *current_weapon_config() const;
        pd::aabb get_weapon_aabb(const pd::config::weapon_config *weapon) const;
        const pd::animation *get_weapon_animation(
            const pd::config::weapon_config *weapon) const;

        void weapon_hit_detection();
        void handle_block_hit(pd::block *block,
                              const pd::config::weapon_config *weapon);
        const pd::vec2 &velocity() const { return m_velocity; }
        void velocity(const pd::vec2 &val) { m_velocity = val; }

        float width() const;
        float height() const;

        void handle_event(SDL_Event &evt);
        void update(pd::timedelta_t dt);
        void draw() const;

    private:
        pd::vec2 m_velocity;
        pd::animation m_thermal_idle_anim;
        pd::animation m_kinetic_idle_anim;
        pd::animation m_electromagnetic_idle_anim;
        pd::animation m_flamethrower_anim;
        pd::animation m_ice_spray_anim;
        stance_type m_stance;
        bool m_alternative_fire;
        float m_movement;
        float m_energy;
        float m_previous_bottom;
        float m_jump_time;
        bool m_tries_jumping;
        bool m_was_jumping;
        bool m_flipped;
        bool m_shooting;
    };
}

#endif
