#include <pd/entity.hpp>
#include <pd/texture.hpp>
#include <pd/drawtools.hpp>


pd::entity::entity(b2World *world, float x, float y)
{
    m_world = world;
    m_x = x;
    m_y = y;
}

void pd::entity::move(float x, float y)
{
    m_x += x;
    m_y += y;
}