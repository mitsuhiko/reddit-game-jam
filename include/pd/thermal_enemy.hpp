#ifndef _INC_PD_THERMAL_ENEMY_HPP_
#define _INC_PD_THERMAL_ENEMY_HPP_
#include <pd/pd.hpp>
#include <pd/enemy.hpp>
#include <pd/animation.hpp>

namespace pd {

    class session;

    class thermal_enemy : public pd::enemy {
    public:
        thermal_enemy(pd::game_session *session, float x = 0.0f, float y = 0.0f);

        void update(pd::timedelta_t dt);
        void local_render(pd::timedelta_t dt) const;

    private:
        stance_type m_stance;
        pd::animation m_walk_anim;
    };
}

#endif
