#ifndef _INC_PD_KINETIC_ENEMY_HPP_
#define _INC_PD_KINETIC_ENEMY_HPP_
#include <pd/pd.hpp>
#include <pd/enemy.hpp>
#include <pd/animation.hpp>

namespace pd {

    class session;

    class kinetic_enemy : public pd::enemy {
    public:
        kinetic_enemy(pd::game_session *session, float x = 0.0f, float y = 0.0f);

        void update(float dt);
        void local_render(float dt) const;

    private:
        stance_type m_stance;
        pd::animation m_walk_anim;
        pd::texture *m_dash_texture;
        bool m_dashing;
        int m_walking_direction;
    };
}

#endif
