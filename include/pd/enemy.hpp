#ifndef _INC_PD_ENEMY_HPP_
#define _INC_PD_ENEMY_HPP_
#include <pd/pd.hpp>
#include <pd/entity.hpp>
#include <pd/player.hpp>

namespace pd {

    class session;

    class enemy : public pd::entity {
    public:
        enemy(pd::game_session *session, float x, float y,
              float width, float height, float base_offset,
              stance_type stance);
        pd::player * player() const { return m_player; }
    private:
        stance_type m_stance;
        pd::player *m_player;
    };
}

#endif
