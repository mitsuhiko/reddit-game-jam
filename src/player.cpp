#include <pd/player.hpp>
#include <pd/texture.hpp>
#include <pd/drawtools.hpp>
#include <pd/game.hpp>
#include <pd/game_session.hpp>
#include <pd/entity.hpp>
#include <pd/config.hpp>
#include <vector>


static const float jump_speed = 120.0f;
static const float movement_speed = 110.0f;
static const float max_movement_speed = 200.0f;
static const float movement_acceleration = 1400.0f;
static const float friction = 5.0f;
static const float max_jump_time = 0.35f;
static const float jump_launch_velocity = -2600.0f;
static const float jump_control_power = 0.14f;


pd::player::player(pd::game_session *session, const pd::vec2 &pos)
    : pd::entity(session, pos, 60.0f, 100.0f),
      m_thermal_idle_anim(pd::get_resource<pd::texture>("textures/character_thermal_idle.png"), 17, 0.05f),
      m_flamethrower_anim(pd::get_resource<pd::texture>("textures/flamer.png"), 5, 0.1f)
{
    m_energy = 1.0f;
    m_movement = 0.0f;
    m_jump_time = 0.0f;
    m_flipped = false;
    m_tries_jumping = false;
    m_was_jumping = false;
    m_shooting = false;
}

void pd::player::apply_physics(float dt)
{
    // gravity and base velocity
    m_velocity.x += m_movement * movement_acceleration * dt;
    m_velocity = pd::apply_gravity(m_velocity, dt);

    // jumping
    if (!m_tries_jumping) {
        m_jump_time = 0.0f;
    } else {
        if ((!m_was_jumping && on_ground()) || m_jump_time > 0.0f)
            m_jump_time += dt;

        if (0.0f < m_jump_time && m_jump_time <= max_jump_time)
            m_velocity.y = jump_launch_velocity * (1.0f -
                pd::pow(m_jump_time / max_jump_time, jump_control_power));
        else
            m_jump_time = 0.0f;
    }
    m_was_jumping = m_tries_jumping;

    m_velocity.x *= 1.0f - friction * dt;
    m_velocity.x = pd::clamp(m_velocity.x, -max_movement_speed,
                             max_movement_speed);

    // position updates
    pd::vec2 old_pos = pos();
    move(m_velocity * dt);

    handle_collisions();
    if (pos().x == old_pos.x)
        m_velocity.x = 0.0f;
    if (pos().y == old_pos.y)
        m_velocity.y = 0.0f;
}

void pd::player::update(pd::timedelta_t dt)
{
    uint8_t *state = SDL_GetKeyboardState(0);

    if (state[SDL_SCANCODE_A] ||
        state[SDL_SCANCODE_LEFT])
        m_movement = -1.0f;
    else if (state[SDL_SCANCODE_D] ||
        state[SDL_SCANCODE_RIGHT])
        m_movement = 1.0f;
    else
        m_movement = 0.0f;

    m_tries_jumping = state[SDL_SCANCODE_SPACE] != 0;
    m_shooting = state[SDL_SCANCODE_LSHIFT] != 0;

    apply_physics(dt);

    m_thermal_idle_anim.update(dt);
    m_flamethrower_anim.update(dt);
    
    if (m_velocity.x < 0)
        m_flipped = true;
    else if (m_velocity.x > 0)
        m_flipped = false;
}

void pd::player::render(pd::timedelta_t dt) const
{
    pd::vec2 pos = this->pos() + pd::vec2(-10.0f, -14.0f);
    draw_effect effect = draw_without_effect;
    if (m_flipped)
        effect = draw_flipped_vertically;

    m_thermal_idle_anim.draw(pos, effect);
    if (m_shooting)
        m_flamethrower_anim.draw(pos + pd::vec2(70.0f, 14.0f), effect);
}
