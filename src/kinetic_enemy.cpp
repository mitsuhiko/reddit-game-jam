#include <pd/kinetic_enemy.hpp>
#include <pd/game.hpp>
#include <pd/texture.hpp>


pd::kinetic_enemy::kinetic_enemy(pd::game_session *session, float x, float y)
    : pd::enemy(session, x, y, 50.0f, 100.0f, 0.0f, thermal_stance),
    m_walk_anim(pd::get_resource<pd::texture>("textures/enemy_kinetic_walk.png"), 19, 0.035f)
{
}

void pd::kinetic_enemy::update(float dt)
{
    m_walk_anim.update(dt);
}

void pd::kinetic_enemy::local_render(float dt) const
{
    m_walk_anim.render();
}