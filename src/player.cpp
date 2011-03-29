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
    m_previous_bottom = 0.0f;
    m_flipped = false;
    m_tries_jumping = false;
    m_was_jumping = false;
    m_on_ground = false;
}

void pd::player::apply_physics(float dt)
{
    // gravity and base velocity
    m_velocity.x += m_movement * movement_acceleration * dt;
    m_velocity.y = pd::clamp(m_velocity.y + pd::gravity_acceleration * dt,
                             -pd::max_fall_speed, pd::max_fall_speed);

    // jumping
    if (!m_tries_jumping) {
        m_jump_time = 0.0f;
    } else {
        if ((!m_was_jumping && m_on_ground) || m_jump_time > 0.0f)
            m_jump_time += dt;

        if (0.0f < m_jump_time && m_jump_time <= max_jump_time)
            m_velocity.y = jump_launch_velocity * (1.0f -
                std::pow(m_jump_time / max_jump_time, jump_control_power));
        else
            m_jump_time = 0.0f;
    }
    m_was_jumping = m_tries_jumping;

    // pseudo drag
    if (m_on_ground)
        m_velocity.x *= ground_drag_factor;
    else
        m_velocity.x *= air_drag_factor;

    // maximum speed
    m_velocity.x = pd::clamp(m_velocity.x, -max_movement_speed, max_movement_speed);

    // apply velocity
    glm::vec2 old_pos = pos();
    pos(pos() + m_velocity * dt);

    handle_collisions();
    if (pos().x == old_pos.x)
        m_velocity.x = 0.0f;
    if (pos().y == old_pos.y)
        m_velocity.y = 0.0f;
}

void pd::player::handle_collisions()
{
    glm::vec2 old_pos = pos();
    pd::aabb bb = bounding_box();
    float tile_width = (float)session()->map()->tile_width();
    float tile_height = (float)session()->map()->tile_height();

    int left_tile = (int)std::floor(bb.left() / tile_width);
    int right_tile = (int)std::ceil(bb.right() / tile_width) - 1;
    int top_tile = (int)std::floor(bb.top() / tile_height);
    int bottom_tile = (int)std::ceil(bb.bottom() / tile_height) - 1;

    m_on_ground = false;

    for (int y = top_tile; y <= bottom_tile; y++) {
        for (int x = left_tile; x <= right_tile; x++) {
            const pd::block *block = session()->map()->get_block(x, y);

            if (!block || block->collision() == pd::block::passable)
                continue;

            pd::aabb block_bb = block->bounding_box();
            glm::vec2 depth = bounding_box().intersection_depth(block_bb);
            if (depth == glm::vec2())
                continue;

            glm::vec2 abs_depth = glm::abs(depth);
            glm::vec2 correction;
            if (abs_depth.y < abs_depth.x ||
                block->collision() == pd::block::semi_passable) {
                if (m_previous_bottom <= block_bb.top())
                    m_on_ground = true;

                if (m_on_ground ||
                    block->collision() == pd::block::impassable)
                    correction.y = depth.y;
            } else if (block->collision() == pd::block::impassable) {
                correction.x = depth.x;
            }

            if (correction != glm::vec2())
                pos(pos() + correction);
        }
    }

    m_previous_bottom = bounding_box().bottom();
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
    m_flamethrower_anim.render(glm::vec2(70.0f, 14.0f));

    pd::pop_matrix();
}
