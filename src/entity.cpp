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
    m_x = x;
    m_y = y;
    m_rotation = 0.0f;
    m_width = width;
    m_height = height;
    m_stance = neutral_stance;
    m_health = 100.0f;
    session->add_entity(this);
}

pd::entity::~entity()
{
}

bool pd::entity::airborne() const
{
    // XXX: implement
    return false;
}

void pd::entity::take_damage(float val, pd::entity::damage_type type)
{
    m_health = std::max(0.0f, m_health - val);
}

void pd::entity::render(float dt) const
{
    pd::push_matrix();
    pd::translate(x() - width() / 2.0f, y() - height() / 2.0f);
    if (m_flipped) {
        pd::scale(-1.0f, 1.0f);
        pd::translate(-width(), 0.0f);
    }
    pd::rotate_around_point(rotation(), width() / 2.0f, height() / 2.0f);
    local_render(dt);
    pd::pop_matrix();
}