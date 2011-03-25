#ifndef _INC_PD_DRAWTOOLS_HPP_
#define _INC_PD_DRAWTOOLS_HPP_
#include <pd/pd.hpp>
#include <pd/color.hpp>

namespace pd {

    class texture;

    void clear_screen(pd::color color);
    void draw_textured_quad(float x, float y, float width, float height, pd::texture *texture);
}

#endif
