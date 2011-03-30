#include <pd/kinetic_enemy.hpp>
#include <pd/game.hpp>
#include <pd/game_session.hpp>
#include <pd/texture.hpp>
#include <pd/drawtools.hpp>
#include <pd/game_session.hpp>

static const float movement_speed = 100.0f;
static const float dash_speed = 700.0f;
static const float dash_countdown = 0.3f;
static const float dash_activation_distance = 300.0f;


pd::kinetic_enemy::kinetic_enemy(pd::game_session *session,
                                 const glm::vec2 &pos)
    : pd::enemy(session, pos, 50.0f, 82.0f),
    m_walk_anim(pd::get_resource<pd::texture>("textures/enemy_kinetic_walk.png"), 19, 0.035f),
    m_dash_anim(pd::get_resource<pd::texture>("textures/enemy_kinetic_dash.png"), 2)
{
    m_dashing = false;
    m_direction = 1;
    m_dash_countdown = -1.0f;
}

void pd::kinetic_enemy::update(pd::timedelta_t dt)
{
    if (m_dash_countdown >= 0.0f) {
        if ((m_dash_countdown -= dt) < 0.0f) {
            m_dashing = true;
            m_dash_countdown = -1.0f;
        }
        return;
    }

    const pd::map *map = session()->map();
    float edge = pos().x + (m_direction > 0 ? width() : 0);
    int tile_x = (int)(edge / map->tile_width()) + m_direction;
    int tile_y = (int)((pos().y + height()) / map->tile_height());
    pd::collision_flag same_coll = map->get_collision(tile_x, tile_y);
    pd::collision_flag bottom_coll = map->get_collision(tile_x, tile_y + 1);

    if (same_coll == pd::impassable || bottom_coll == pd::passable)
        m_direction *= -1;

    m_walk_anim.update(dt);
    float speed = m_dashing ? dash_speed : movement_speed;
    move(glm::vec2(m_direction * speed, 0.0f) * dt);
}

bool pd::kinetic_enemy::starts_dashing() const
{
    return false;
}

void pd::kinetic_enemy::render(pd::timedelta_t dt) const
{
    pd::push_matrix();
    pd::translate(pos());

    if (m_direction < 0.0f) {
        pd::scale(glm::vec2(-1.0f, 1.0f));
        pd::translate(glm::vec2(-width(), 0.0f));
    }

    if (m_dash_countdown >= 0.0f || m_dashing) {
        m_dash_anim.render_frame(m_dashing ? 1 : 0, glm::vec2(60.0f, 0.0f));
    } else {
        m_walk_anim.render();
    }

    pd::pop_matrix();
}
