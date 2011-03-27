#include <pd/enemy.hpp>
#include <pd/texture.hpp>
#include <pd/drawtools.hpp>
#include <pd/game.hpp>


pd::enemy::enemy(pd::game_session *session, float x, float y,
                 float width, float height, float base_offset,
                 stance_type stance)
    : pd::entity(session, x, y, width, height, base_offset,
                 20.0f, 0.8f, true)
{
    this->stance(stance);
}