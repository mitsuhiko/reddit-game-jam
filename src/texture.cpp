#include <pd/texture.hpp>
#include <pd/image.hpp>
#include <pd/math.hpp>


pd::texture *pd::texture::slice(int x, int y, int width, int height)
{
    return new pd::texture_slice(this, x, y, width, height);
}

pd::texture *pd::texture_from_surface(SDL_Surface *surface)
{
    return new pd::simple_texture(surface);
}

pd::texture *pd::load_texture(const std::string &filename)
{
    SDL_Surface *surface = pd::load_image(filename);
    pd::texture *rv = new pd::simple_texture(surface);
    SDL_FreeSurface(surface);
    return rv;
}

pd::simple_texture::simple_texture(SDL_Surface *surface)
{
    m_width = surface->w;
    m_height = surface->h;

    if (SDL_GL_ExtensionSupported("GL_ARB_texture_non_power_of_two")) {
        m_stored_width = m_width;
        m_stored_height = m_height;
    } else {
        m_stored_width = pd::next_power_of_two(m_width);
        m_stored_height = pd::next_power_of_two(m_height);
    }
    
    glGenTextures(1, &m_id);
    glBindTexture(GL_TEXTURE_2D, m_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_stored_width,
                 m_stored_height, 0, GL_RGBA, GL_UNSIGNED_BYTE,
                 surface->pixels);
}

pd::simple_texture::~simple_texture()
{
    glDeleteTextures(1, &m_id);
}

pd::texture_slice::texture_slice(pd::texture *parent, int x, int y, int width, int height)
{
    m_parent = parent;
    m_offset_x = x;
    m_offset_y = y;
    m_width = width;
    m_height = height;
}