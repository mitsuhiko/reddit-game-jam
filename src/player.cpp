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
static const float max_movement_speed = 1000.0f;
static const float movement_acceleration = 14000.0f;
static const float ground_drag_factor = 0.58f;
static const float air_drag_factor = 0.45f;
static const float max_jump_time = 0.35f;
static const float jump_launch_velocity = -2600.0f;
static const float jump_control_power = 0.14f;


pd::player::player(pd::game_session *session, const glm::vec2 &pos)
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
                std::pow(m_jump_time / max_jump_time, jump_control_power));
        else
            m_jump_time = 0.0f;
    }
    m_was_jumping = m_tries_jumping;

    // pseudo drag
    if (on_ground())
        m_velocity.x *= ground_drag_factor;
    else
        m_velocity.x *= air_drag_factor;

    m_velocity.x = pd::clamp(m_velocity.x, -max_movement_speed,
                             max_movement_speed);

    // position updates
    glm::vec2 old_pos = pos();
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
    static const glm::vec2 hovering(0.0f, -14.0f);

    pd::push_matrix();
    pd::translate(pos() + hovering);

    if (m_flipped) {
        pd::scale(glm::vec2(-1.0f, 1.0f));
        pd::translate(glm::vec2(-width(), 0.0f));
    }

    m_thermal_idle_anim.render();
    if (m_shooting)
        m_flamethrower_anim.render(glm::vec2(70.0f, 14.0f));

    pd::pop_matrix();
}
