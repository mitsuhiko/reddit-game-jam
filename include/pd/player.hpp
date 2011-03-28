#ifndef _INC_PD_PLAYER_HPP_
#define _INC_PD_PLAYER_HPP_
#include <pd/pd.hpp>
#include <pd/entity.hpp>
#include <pd/animation.hpp>

namespace pd {

    class session;

    class player : public pd::entity {
    public:
        player(pd::game_session *session, float x = 0.0f, float y = 0.0f);
        float energy() const { return m_energy; }
        void shooting(bool val) { m_shooting = m_energy > 0.0f && val; }
        bool shooting() const { return m_shooting; }
        
        void move_left(pd::timedelta_t dt);
        void move_right(pd::timedelta_t dt);
        void jump();

        void take_damage(float val, damage_type type);
        void update(pd::timedelta_t dt);
        void local_render(pd::timedelta_t dt) const;

    private:
        pd::animation m_thermal_idle_anim;
        pd::animation m_flamethrower_anim;
        float m_energy;
        bool m_shooting;
    };
}

#endif
