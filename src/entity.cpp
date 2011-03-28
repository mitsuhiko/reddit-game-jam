#include <pd/entity.hpp>
#include <pd/texture.hpp>
#include <pd/drawtools.hpp>
#include <pd/game.hpp>
#include <pd/game_session.hpp>
#include <pd/config.hpp>


pd::entity::entity(pd::game_session *session, const glm::vec2 &pos, float width,
                   float height)
{
    m_session = session;
    m_pos = pos;
    m_width = width;
    m_height = height;
}

pd::entity::~entity()
{
}

pd::aabb pd::entity::bounding_box() const
{
    return pd::aabb(m_pos, m_pos + glm::vec2(m_width, m_height));
}