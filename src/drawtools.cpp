#include <pd/drawtools.hpp>
#include <pd/texture.hpp>
#include <pd/font.hpp>
#include <pd/aabb.hpp>
#include <vector>


void pd::clear_screen(pd::color color)
{
    float col[4];
    color.to_float(col);
    glClearColor(col[0], col[1], col[2], col[3]);
    glClear(GL_COLOR_BUFFER_BIT);
}

void pd::draw_quad(const pd::texture *texture)
{
    draw_quad(texture, pd::vec2(0.0f, 0.0f),
              (float)texture->width(), (float)texture->height());
}

void pd::draw_quad(const pd::texture *texture, const pd::vec2 &pos,
                   draw_effect effect, pd::color color)
{
    draw_quad(texture, pos, (float)texture->width(), (float)texture->height(),
              effect, color);
}

void pd::draw_quad(const pd::texture *texture, const pd::vec2 &pos,
                   float width, float height, draw_effect effect,
                   pd::color color)
{
    float x = pos.x;
    float y = pos.y;
    glBindTexture(GL_TEXTURE_2D, texture->id());
    float vertices[] = {
        x, y,
        x, y + height,
        x + width, y + height,
        x + width, y
    };
    float fac_x = (float)texture->width() / texture->stored_width();
    float fac_y = (float)texture->height() / texture->stored_height();
    float off_x = (float)texture->offset_x() / texture->stored_width();
    float off_y = (float)texture->offset_y() / texture->stored_height();
    float texcoords[] = {
        off_x, off_y,
        off_x, fac_y + off_y,
        fac_x + off_x, fac_y + off_y,
        fac_x + off_x, off_y
    };

    if ((effect & draw_flipped_horizontally) != 0) {
        pd::swap(texcoords[1], texcoords[3]);
        pd::swap(texcoords[5], texcoords[7]);
    }
    if ((effect & draw_flipped_vertically) != 0) {
        pd::swap(texcoords[0], texcoords[4]);
        pd::swap(texcoords[2], texcoords[6]);
    }

    float col[4];
    color.to_float(col);
    glColor4fv(col);
    glVertexPointer(2, GL_FLOAT, 0, vertices);
    glTexCoordPointer(2, GL_FLOAT, 0, texcoords);
    glDrawArrays(GL_QUADS, 0, 4);
}

void pd::draw_text(const std::string &text, const pd::vec2 &pos,
                   const pd::font *font, pd::color color)
{
    pd::vec2 cur_pos = pos;

    for (size_t i = 0; i < text.size(); i++) {
        unsigned char c = text[i];
        if (c == '\n') {
            cur_pos.x = pos.x;
            cur_pos.y += font->height();
            continue;
        }

        const glyph_info &glyph = font->get(c);
        if (glyph.texture)
            draw_quad(glyph.texture,
                      cur_pos + pd::vec2(glyph.xoff, glyph.yoff),
                      draw_without_effect, color);
        cur_pos.x += glyph.advance;
    }
}

void pd::draw_debug_box(const pd::vec2 &pos, float width, float height,
                        pd::color color)
{
    float x = pos.x;
    float y = pos.y;
    float vertices[] = {
        x, y,
        x, y + height,
        x + width, y + height,
        x + width, y
    };

    glBindTexture(GL_TEXTURE_2D, 0);
    float col[4];
    color.to_float(col);
    glColor4fv(col);
    glVertexPointer(2, GL_FLOAT, 0, vertices);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDrawArrays(GL_LINE_LOOP, 0, 4);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
}

void pd::draw_debug_box(const pd::aabb &aabb, pd::color color)
{
    draw_debug_box(aabb.v1, aabb.width(), aabb.height(), color);
}
