#include <pd/sprite.hpp>
#include <pd/texture.hpp>
#include <pd/drawtools.hpp>


pd::sprite::sprite(const pd::texture *texture, float x, float y)
{
    m_x = x;
    m_y = y;
    m_texture = texture;
}

void pd::sprite::move(float x, float y)
{
    m_x += x;
    m_y += y;
}

void pd::sprite::render(float dt) const
{
    pd::draw_textured_quad(m_x, m_y, (float)m_texture->width(),
        (float)m_texture->height(), m_texture);
}