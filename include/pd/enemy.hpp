#ifndef _INC_PD_ENEMY_HPP_
#define _INC_PD_ENEMY_HPP_
#include <pd/pd.hpp>
#include <pd/entity.hpp>

namespace pd {

    class session;

    class enemy : public pd::entity {
    public:
        enemy(pd::game_session *session, float x, float y,
              float width, float height, float base_offset,
              stance_type stance);

    private:
        stance_type m_stance;
    };
}

#endif
