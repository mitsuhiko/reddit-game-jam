#include <pd/kinetic_enemy.hpp>
#include <pd/game.hpp>
#include <pd/game_session.hpp>
#include <pd/texture.hpp>
#include <pd/drawtools.hpp>
#include <pd/game_session.hpp>
#include <pd/config.hpp>
#include <pd/rnd.hpp>


static pd::config::_kinetic_enemy &cfg = pd::config::kinetic_enemy;


pd::kinetic_enemy::kinetic_enemy(pd::game_session *session,
                                 const pd::vec2 &pos)
    : pd::enemy(session, pos),
    m_walk_anim(cfg.walk), m_dash_anim(cfg.dash)
{
    m_state = spawn_state;
    m_direction = 1;
    m_state_countdown = 0.0f;
}

float pd::kinetic_enemy::width() const
{
    return cfg.width;
}

float pd::kinetic_enemy::height() const
{
    return cfg.height;
}

bool pd::kinetic_enemy::movement_frozen() const
{
    switch (m_state) {
    case spawn_state:
    case prepare_dashing_state:
    case dash_recover_state:
        return true;
    default:
        return false;
    }
}

bool pd::kinetic_enemy::dashing() const
{
    return m_state == dashing_state;
}

float pd::kinetic_enemy::current_speed() const
{
    if (movement_frozen())
        return 0.0f;
    return dashing() ? cfg.dash_speed : cfg.movement_speed;
}

void pd::kinetic_enemy::update(pd::timedelta_t dt)
{
    m_state_countdown -= dt;

    m_velocity.x = m_direction * current_speed();
    m_velocity = pd::apply_gravity(m_velocity, dt);

    switch (m_state) {
    case spawn_state:
        break;
    case prepare_dashing_state:
        if (m_state_countdown < 0.0f)
            m_state = dashing_state;
        break;
    case walking_state:
        if (can_see(session()->player())) {
            m_state_countdown = cfg.dash_countdown;
            m_state = prepare_dashing_state;
        }
        break;
    case dashing_state:
        break;
    case dash_recover_state:
        if (m_state_countdown < 0.0f) {
            m_direction *= -1;
            m_state = walking_state;
        }
        return;
    }

    m_walk_anim.update(dt);

    pd::vec2 old_pos = pos();
    move(m_velocity * dt);
    handle_collisions();

    if (pos().y == old_pos.y) {
        m_velocity.y = 0.0f;
        if (m_state == spawn_state)
            m_state = walking_state;
    }

    if (m_velocity.x && pos().x == old_pos.x) {
        if (m_state == dashing_state) {
            m_state_countdown = pd::default_rnd().range(0.1f, 0.3f);
            m_state = dash_recover_state;
        } else {
            m_state = walking_state;
            if (pd::sign(m_velocity.x) == pd::sign(m_direction))
                m_direction *= -1;
        }
    }
}

bool pd::kinetic_enemy::can_see(const pd::entity *other) const
{
    pd::aabb sight_box = pd::aabb::make_box(bounding_box().center_top(),
                                            cfg.see_distance * m_direction,
                                            height());
    return other->bounding_box().intersects(sight_box);
}

void pd::kinetic_enemy::render(pd::timedelta_t dt) const
{
    const pd::vec2 pos = this->pos() - cfg.default_offset;
    draw_effect effect = draw_without_effect;
    if (m_direction < 0.0f)
        effect = draw_flipped_vertically;

    if (m_state == prepare_dashing_state ||
        m_state == dashing_state ||
        m_state == dash_recover_state)
        m_dash_anim.draw_frame(dashing() ? 1 : 0,
                               pos - cfg.dash_offset, effect);
    else
        m_walk_anim.draw(pos, effect);
}
