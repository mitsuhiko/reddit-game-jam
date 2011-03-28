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
        
        void update(pd::timedelta_t dt);
        void render(pd::timedelta_t dt) const;

    private:
        pd::animation m_thermal_idle_anim;
        pd::animation m_flamethrower_anim;
        float m_energy;
        pd::timedelta_t m_movement_time;
    };
}

#endif
