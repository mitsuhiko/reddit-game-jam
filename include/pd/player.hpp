#ifndef _INC_PD_PLAYER_HPP_
#define _INC_PD_PLAYER_HPP_
#include <pd/pd.hpp>
#include <pd/entity.hpp>
#include <pd/animation.hpp>

namespace pd {

    class session;

    class player : public pd::entity {
    public:
        enum stance_type {
            kinetic_stance,
            thermal_stance,
            electromagnetic_stance
        };

        player(pd::game_session *session, const glm::vec2 &pos);

        float energy() const { return m_energy; }
        stance_type stance() const { return thermal_stance; }
        bool looks_right() const { return !m_flipped; }
        bool looks_left() const { return m_flipped; }
        
        void apply_physics(pd::timedelta_t dt);
        const glm::vec2 &velocity() const { return m_velocity; }
        void velocity(const glm::vec2 &val) { m_velocity = val; }

        void update(pd::timedelta_t dt);
        void render(pd::timedelta_t dt) const;

    private:
        glm::vec2 m_velocity;
        pd::animation m_thermal_idle_anim;
        pd::animation m_flamethrower_anim;
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
