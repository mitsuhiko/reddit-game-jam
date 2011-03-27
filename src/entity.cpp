#include <pd/entity.hpp>
#include <pd/texture.hpp>
#include <pd/drawtools.hpp>
#include <pd/game.hpp>
#include <pd/game_session.hpp>

pd::entity::entity(pd::game_session *session, float x, float y, float width,
                   float height)
{
    m_session = session;
    m_flipped = false;
    m_pos = glm::vec2(x, y);
    m_rotation = 0.0f;
    m_width = width;
    m_height = height;
    m_stance = neutral_stance;
    m_health = 100.0f;
    m_hovering = 0.0f;
    m_air_time = 0.0f;
    session->add_entity(this);
}

pd::entity::~entity()
{
}

bool pd::entity::can_see(const pd::entity *entity, float max_distance)
{
    // XXX: actual raycasts would be good
    glm::vec2 distance = entity->pos() - pos();
    if (glm::length(distance) > max_distance)
        return false;
    return pd::sign(distance.x) == (flipped() ? -1 : 1);
}

bool pd::entity::collides_left() const
{
    const pd::map *map = session()->map();

    for (float yoff = 0.0f; yoff < m_height; yoff++) {
        int tile_x = (int)(x() / map->tile_width()) - 1;
        int tile_y = (int)((y() + yoff) / map->tile_height()) - 1;
        if (map->occupied(tile_x, tile_y))
            return true;
    }

    return false;
}

bool pd::entity::collides_right() const
{
    const pd::map *map = session()->map();

    for (float yoff = 0.0f; yoff < m_height; yoff++) {
        int tile_x = (int)((x() + m_width) / map->tile_width()) - 1;
        int tile_y = (int)((y() + yoff) / map->tile_height()) - 1;
        if (map->occupied(tile_x, tile_y))
            return true;
    }

    return false;
}

bool pd::entity::airborne() const
{
    const pd::map *map = session()->map();
    int tile_x = (int)(x() / map->tile_width());
    int tile_y = (int)((y() + m_height) / map->tile_height()) - 1;

    return !map->occupied(tile_x, tile_y);
}

void pd::entity::take_damage(float val, pd::entity::damage_type type)
{
    m_health = std::max(0.0f, m_health - val);
}

void pd::entity::apply_gravity(float dt)
{
    if (airborne()) {
        m_air_time += dt;
        pd::apply_gravity(m_velocity, m_air_time);
    } else {
        m_air_time = 0.0f;
        if (m_velocity.y > 0.0f)
            m_velocity.y = 0.0f;
    }
}

void pd::entity::render(float dt) const
{
    pd::push_matrix();
    pd::translate(x() - width() / 2.0f, y() - height() / 2.0f - m_hovering);
    if (m_flipped) {
        pd::scale(-1.0f, 1.0f);
        pd::translate(-width(), 0.0f);
    }
    pd::rotate_around_point(rotation(), width() / 2.0f, height() / 2.0f);
    local_render(dt);
    pd::pop_matrix();
}