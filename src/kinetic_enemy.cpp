#include <pd/kinetic_enemy.hpp>
#include <pd/game.hpp>
#include <pd/game_session.hpp>
#include <pd/texture.hpp>
#include <pd/drawtools.hpp>
#include <pd/game_session.hpp>
#include <pd/rnd.hpp>

static const float spawn_fall_speed = 300.0f;
static const float movement_speed = 100.0f;
static const float dash_speed = 500.0f;
static const float dash_countdown = 0.3f;
static const float see_distance = 150.0f;


pd::kinetic_enemy::kinetic_enemy(pd::game_session *session,
                                 const glm::vec2 &pos)
    : pd::enemy(session, pos, 50.0f, 82.0f),
    m_walk_anim(pd::get_resource<pd::texture>("textures/enemy_kinetic_walk.png"), 19, 0.035f),
    m_dash_anim(pd::get_resource<pd::texture>("textures/enemy_kinetic_dash.png"), 2)
{
    m_state = spawn_state;
    m_direction = 1;
    m_state_countdown = 0.0f;
    m_hit_player = false;
}

void pd::kinetic_enemy::update(pd::timedelta_t dt)
{
    const pd::map *map = session()->map();
    float edge = pos().x + (m_direction > 0 ? width() : 0);
    int tile_x = (int)(edge / map->tile_width()) + m_direction;
    int tile_y = (int)((pos().y + height()) / map->tile_height());
    m_state_countdown -= dt;

    switch (m_state) {
    case spawn_state: {
        float dy = spawn_fall_speed * dt;
        float new_bottom = (pos().y + dy + height());
        tile_y = (int)(new_bottom / map->tile_height());
        pd::collision_flag coll = map->get_collision(tile_x, tile_y);
        if (coll == pd::passable)
            move(glm::vec2(0.0f, dy));
        else
            m_state = walking_state;
        return;
    }
    case turning_state:
        if (m_state_countdown < 0.0f) {
            m_state = walking_state;
            m_direction *= -1;
        }
        return;
    case prepare_dashing_state:
        if (m_state_countdown < 0.0f) {
            m_state = dashing_state;
            m_hit_player = false;
        }
        return;
    case walking_state:
        if (can_see(session()->player())) {
            m_state_countdown = dash_countdown;
            m_state = prepare_dashing_state;
            return;
        }
        break;
    case dashing_state:
        if (!m_hit_player &&
            bounding_box().intersects(session()->player()->bounding_box())) {
            m_hit_player = true;
            // TODO: move and damage player
        }
        break;
    }

    pd::collision_flag same_coll = map->get_collision(tile_x, tile_y);
    pd::collision_flag bottom_coll = map->get_collision(tile_x, tile_y + 1);

    if (same_coll == pd::impassable || bottom_coll == pd::passable) {
        m_state_countdown = pd::default_rnd().range(0.2f, 0.4f);
        m_state = turning_state;
    }

    m_walk_anim.update(dt);
    float speed = dashing() ? dash_speed : movement_speed;
    move(glm::vec2(m_direction * speed, 0.0f) * dt);
}

bool pd::kinetic_enemy::can_see(const pd::entity *other) const
{
    pd::aabb sight_box = pd::aabb::make_box(bounding_box().center_top(),
                                            see_distance * m_direction,
                                            height());
    return other->bounding_box().intersects(sight_box);
}

void pd::kinetic_enemy::render(pd::timedelta_t dt) const
{
    pd::push_matrix();
    pd::translate(pos());

    if (m_direction < 0.0f) {
        pd::scale(glm::vec2(-1.0f, 1.0f));
        pd::translate(glm::vec2(-width(), 0.0f));
    }

    if (m_state == prepare_dashing_state || m_state == dashing_state) {
        m_dash_anim.render_frame(dashing() ? 1 : 0, glm::vec2(-60.0f, 0.0f));
    } else {
        m_walk_anim.render();
    }

    pd::pop_matrix();
}
