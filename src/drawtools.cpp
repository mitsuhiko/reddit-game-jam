#include <pd/drawtools.hpp>
#include <pd/texture.hpp>


void pd::clear_screen(pd::color color)
{
    float col[4];
    color.to_float(col);
    glClearColor(col[0], col[1], col[2], col[3]);
    glClear(GL_COLOR_BUFFER_BIT);
}

void pd::draw_textured_quad(float x, float y, float width, float height, pd::texture *texture)
{
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