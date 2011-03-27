#include <pd/thermal_enemy.hpp>
#include <pd/game.hpp>
#include <pd/texture.hpp>


pd::thermal_enemy::thermal_enemy(pd::game_session *session, float x, float y)
    : pd::enemy(session, x, y, 36.0f, 40.0f, 20.0f, thermal_stance),
    m_walk_anim(pd::get_resource<pd::texture>("textures/test-enemy.png"), 4, 0.2f)
{
}

void pd::thermal_enemy::update(float dt)
{
    m_walk_anim.update(dt);
}

void pd::thermal_enemy::local_render(float dt) const
{
    m_walk_anim.render(dt);
}