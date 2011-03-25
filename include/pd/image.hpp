#ifndef _INC_PD_IMAGE_HPP_
#define _INC_PD_IMAGE_HPP_
#include <pd/pd.hpp>

namespace pd {

    SDL_Surface *load_image(const std::string &filename);
}

#endif
