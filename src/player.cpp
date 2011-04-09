#include <pd/player.hpp>
#include <pd/texture.hpp>
#include <pd/drawtools.hpp>
#include <pd/game.hpp>
#include <pd/game_session.hpp>
#include <pd/entity.hpp>
#include <pd/config.hpp>
#include <vector>


static pd::config::_player &cfg = pd::config::player;


pd::player::player(pd::game_session *session, const pd::vec2 &pos)
    : pd::entity(session, pos), m_thermal_idle_anim(cfg.thermal_idle),
      m_kinetic_idle_anim(cfg.kinetic_idle),
      m_electromagnetic_idle_anim(cfg.electromagnetic_idle),
      m_flamethrower_anim(cfg.flamethrower)
{
    m_stance = thermal_stance;
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
    float acceleration = m_stance == kinetic_stance ?
        cfg.kinetic_movement_acceleration : cfg.movement_acceleration;
    float max_speed = m_stance == kinetic_stance ?
        cfg.max_kinetic_movement_speed : cfg.max_movement_speed;

    // gravity and base velocity
    m_velocity.x += m_movement * acceleration * dt;
    m_velocity = pd::apply_gravity(m_velocity, dt);

    // jumping
    if (!m_tries_jumping) {
        m_jump_time = 0.0f;
    } else {
        if ((!m_was_jumping && on_ground()) || m_jump_time > 0.0f)
            m_jump_time += dt;

        if (0.0f < m_jump_time && m_jump_time <= cfg.max_jump_time)
            m_velocity.y = cfg.jump_launch_velocity * (1.0f -
                pd::pow(m_jump_time / cfg.max_jump_time, cfg.jump_control_power));
        else
            m_jump_time = 0.0f;
    }
    m_was_jumping = m_tries_jumping;

    m_velocity.x *= 1.0f - cfg.friction * dt;
    m_velocity.x = pd::clamp(m_velocity.x, -max_speed, max_speed);

    // position updates
    pd::vec2 old_pos = pos();
    move(m_velocity * dt);

    handle_collisions();
    if (pos().x == old_pos.x)
        m_velocity.x = 0.0f;
    if (pos().y == old_pos.y)
        m_velocity.y = 0.0f;
}

float pd::player::width() const
{
    return cfg.width;
}

float pd::player::height() const
{
    return cfg.height;
}

void pd::player::handle_event(SDL_Event &evt, pd::timedelta_t dt)
{
    if (evt.type == SDL_KEYDOWN) {
        switch (evt.key.keysym.sym) {
        case SDLK_1:
            m_stance = thermal_stance;
            break;
        case SDLK_2:
            m_stance = kinetic_stance;
            break;
        case SDLK_3:
            m_stance = electromagnetic_stance;
            break;
        }
    }
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
    m_kinetic_idle_anim.update(dt);
    m_electromagnetic_idle_anim.update(dt);
    m_flamethrower_anim.update(dt);
    
    if (m_velocity.x < 0)
        m_flipped = true;
    else if (m_velocity.x > 0)
        m_flipped = false;
}

const pd::animation *pd::player::current_animation() const
{
    switch (m_stance) {
    case thermal_stance:
        return &m_thermal_idle_anim;
    case kinetic_stance:
        return &m_kinetic_idle_anim;
    case electromagnetic_stance:
        return &m_electromagnetic_idle_anim;
    default:
        assert(0);
        return 0;
    }
}

const pd::vec2 &pd::player::bound_offset() const
{
    static glm::vec2 dummy_vec;
    switch (m_stance) {
    case thermal_stance:
        return cfg.thermal_offset;
    case kinetic_stance:
        return cfg.kinetic_offset;
    case electromagnetic_stance:
        return cfg.electromagnetic_offset;
    default:
        assert(0);
        return dummy_vec;
    }
}

void pd::player::render(pd::timedelta_t dt) const
{
    pd::push_matrix();
    pd::vec2 pos = this->pos();
    const pd::animation *anim = current_animation();

    pos -= bound_offset();

    pd::translate(pos);
    if (m_flipped) {
        pd::scale(-1.0f, 1.0f);
        pd::translate(-(float)anim->width(), 0.0f);
    }

    anim->draw();

    if (m_shooting)
        m_flamethrower_anim.draw(cfg.flamethrower_offset);

    pd::pop_matrix();
}
