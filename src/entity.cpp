#include <pd/entity.hpp>
#include <pd/texture.hpp>
#include <pd/drawtools.hpp>


pd::entity::entity(float x, float y)
{
    m_x = x;
    m_y = y;
}

void pd::entity::move(float x, float y)
{
    m_x += x;
    m_y += y;
}