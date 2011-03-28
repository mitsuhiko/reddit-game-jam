#include <pd/drawtools.hpp>
#include <pd/texture.hpp>
#include <pd/font.hpp>
#include <vector>


void pd::reset_color()
{
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}

void pd::set_color(pd::color color)
{
    float col[4];
    color.to_float(col);
    glColor4fv(col);
}

void pd::clear_screen(pd::color color)
{
    float col[4];
    color.to_float(col);
    glClearColor(col[0], col[1], col[2], col[3]);
    glClear(GL_COLOR_BUFFER_BIT);
}

void pd::draw_textured_quad(const glm::vec2 &pos, float width, float height, const pd::texture *texture)
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

    glVertexPointer(2, GL_FLOAT, 0, vertices);
    glTexCoordPointer(2, GL_FLOAT, 0, texcoords);
    glDrawArrays(GL_QUADS, 0, 4);
}

void pd::draw_textured_quad(const glm::vec2 &pos, const pd::texture *texture)
{
    pd::draw_textured_quad(pos, (float)texture->width(), (float)texture->height(), texture);
}

void pd::draw_text(const std::string &text, const glm::vec2 &pos, const pd::font *font)
{
    glm::vec2 cur_pos = pos;

    /* XXX: write the quads into a vector and draw them at once */
    for (size_t i = 0; i < text.size(); i++) {
        unsigned char c = text[i];
        if (c == '\n') {
            cur_pos.x = pos.x;
            cur_pos.y += font->height();
            continue;
        }

        const glyph_info &glyph = font->get(c);
        if (glyph.texture)
            draw_textured_quad(cur_pos + glm::vec2(glyph.xoff, glyph.yoff),
                               (float)glyph.texture->width(),
                               (float)glyph.texture->height(),
                               glyph.texture);
        cur_pos.x += glyph.advance;
    }
}