#ifndef _INC_PD_DRAWTOOLS_HPP_
#define _INC_PD_DRAWTOOLS_HPP_
#include <pd/pd.hpp>
#include <pd/color.hpp>

namespace pd {

    class texture;
    class font;

    void clear_screen(pd::color color);
    void draw_textured_quad(float x, float y, float width, float height, const pd::texture *texture);
    void draw_textured_quad(float x, float y, const pd::texture *texture);
    void draw_text(const std::string &text, float x, float y, const pd::font *font);
}

#endif
