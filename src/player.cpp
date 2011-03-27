#include <pd/player.hpp>
#include <pd/texture.hpp>
#include <pd/drawtools.hpp>
#include <pd/game.hpp>


static const float max_airborne_velocity = 3.0f;
static const float max_velocity = 6.0f;
static const float jump_impulse = 1250.0f;
static const float movement_force = 4000.0f;

pd::player::player(pd::game_session *session, float x, float y)
    : pd::entity(session, x, y, 60.0f, 90.0f, -20.0f, 20.0f, 0.0f, true),
      m_thermal_idle_anim(pd::get_resource<pd::texture>("textures/character_thermal_idle.png"), 17, 0.05f),
      m_flamethrower_anim(pd::get_resource<pd::texture>("textures/flamer.png"), 5, 0.1f)
{
    stance(kinetic_stance);

    m_energy = 1.0f;
    m_shooting = false;
}

void pd::player::move_left()
{
    m_ticks_until_stop = SDL_GetTicks();
    flipped(true);
    pd::vec2 vec = linear_velocity();
    if (vec.x > -(airborne() ? max_airborne_velocity : max_velocity))
         apply_force(-movement_force, 0.0f);
}

void pd::player::move_right()
{
    m_ticks_until_stop = SDL_GetTicks();
    flipped(false);
    pd::vec2 vec = linear_velocity();
    if (vec.x < (airborne() ? max_airborne_velocity : max_velocity))
        apply_force(movement_force, 0.0f);
}

void pd::player::jump()
{
    if (!airborne())
        apply_impulse(0.0f, -jump_impulse);
}

void pd::player::update(float dt)
{
    apply_force(0.0f, 800.0f);
    m_thermal_idle_anim.update(dt);
    m_flamethrower_anim.update(dt);

    if (m_shooting) {
        m_energy = std::max(0.0f, m_energy - dt * 0.35f);
        if (m_energy == 0.0f)
            m_shooting = false;
    }
}

void pd::player::local_render(float dt) const
{    
    m_thermal_idle_anim.render();
    if (m_shooting)
        m_flamethrower_anim.render(70.0f, 14.0f);
}

void pd::player::stop()
{
    if (((SDL_GetTicks() - m_ticks_until_stop) > 100) && !airborne())
        body()->SetLinearVelocity(b2Vec2(0.0f, body()->GetLinearVelocity().y));
}
