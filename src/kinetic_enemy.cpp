#include <pd/kinetic_enemy.hpp>
#include <pd/game.hpp>
#include <pd/texture.hpp>
#include <pd/drawtools.hpp>

pd::kinetic_enemy::kinetic_enemy(pd::game_session *session, float x, float y)
    : pd::enemy(session, x, y, 50.0f, 90.0f, 0.0f, thermal_stance),
    m_walk_anim(pd::get_resource<pd::texture>("textures/enemy_kinetic_walk.png"), 19, 0.035f)
{
    m_dash_texture = pd::get_resource<pd::texture>("textures/enemy_kinetic_dash.png");
    m_walking_direction = -1;
    m_dashing = false;
}

void pd::kinetic_enemy::update(float dt)
{
    m_walk_anim.update(dt);

    // More clever than Skynet!
    if (body()->GetLinearVelocity().x < 0.1f && body()->GetLinearVelocity().x > - 0.1f) {
        m_walking_direction *= -1;
        m_dashing = 0;
        apply_impulse(600.0f * m_walking_direction, 0.0f);
    }
    
    
    /* else {
        apply_force(700.0f * m_walking_direction, 0.0f);
        if (((x() - player()->x()) * (x() - player()->x()) < (float)(500 ^ 2)) && !m_dashing) {
            apply_impulse(3000.0f * m_walking_direction, 0);
            printf("Impulse!/n");
            m_dashing = 1;
        }
    }
    */
    apply_force(800.0f * m_walking_direction, 0.0f);
}

void pd::kinetic_enemy::local_render(float dt) const
{
    m_walk_anim.render();
}