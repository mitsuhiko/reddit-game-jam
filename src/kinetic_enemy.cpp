#include <pd/kinetic_enemy.hpp>
#include <pd/game.hpp>
#include <pd/texture.hpp>
#include <pd/drawtools.hpp>
#include <pd/game_session.hpp>


pd::kinetic_enemy::kinetic_enemy(pd::game_session *session, float x, float y)
    : pd::enemy(session, x, y, 50.0f, 90.0f, 0.0f, thermal_stance),
    m_walk_anim(pd::get_resource<pd::texture>("textures/enemy_kinetic_walk.png"), 19, 0.035f)
{
    m_dash_texture = pd::get_resource<pd::texture>("textures/enemy_kinetic_dash.png");
    m_walking_direction = 1;
    m_dashing = false;
}

void pd::kinetic_enemy::update(float dt)
{
    m_walk_anim.update(dt);

    // Smarter than Skynet!
    if (colliding_right()) {
        m_walking_direction = -1;
        apply_impulse(-500.0f, 0.0f);
        m_dashing = false;
    } else if (colliding_left()) {
        m_walking_direction = 1;
        apply_impulse(500.0f, 0.0f);
        m_dashing = false;
    }

    if (can_see(session()->player(), 200.0f)) {
        m_dashing = true;
        apply_impulse(2000.0f, 0.0f);
    }
   
    apply_force((m_dashing ? 4000.0f : 1200.0f) * m_walking_direction, 0.0f);
    flipped(linear_velocity().x < 0.0f);
}

void pd::kinetic_enemy::local_render(float dt) const
{
    if (m_dashing)
        pd::draw_textured_quad(m_dash_texture);
    else
        m_walk_anim.render();
}