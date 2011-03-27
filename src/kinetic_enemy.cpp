#include <pd/kinetic_enemy.hpp>
#include <pd/game.hpp>
#include <pd/texture.hpp>
#include <pd/drawtools.hpp>
#include <pd/game_session.hpp>


pd::kinetic_enemy::kinetic_enemy(pd::game_session *session, float x, float y)
    : pd::enemy(session, x, y, 50.0f, 90.0f, thermal_stance),
    m_walk_anim(pd::get_resource<pd::texture>("textures/enemy_kinetic_walk.png"), 19, 0.035f),
    m_dash_anim(pd::get_resource<pd::texture>("textures/enemy_kinetic_dash.png"), 2)
{
    m_dashing = false;
    m_dash_countdown = -1.0f;
}

void pd::kinetic_enemy::update(float dt)
{
    m_walk_anim.update(dt);
}

void pd::kinetic_enemy::local_render(float dt) const
{
    if (m_dash_countdown > 0.0f)
        m_dash_anim.render_frame(0);
    else if (m_dashing)
        m_dash_anim.render_frame(1);
    else
        m_walk_anim.render();
}