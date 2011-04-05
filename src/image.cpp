#include <pd/image.hpp>
#include <sstream>

/* this is the only file using stb_image, so it's fine */
#include <stb_image.c>


SDL_Surface *pd::load_image(const std::string &filename)
{
    FILE *file = fopen(filename.c_str(), "rb");
    if (!file) {
        std::stringstream ss;
        ss << "Could not open image '" << filename << "'";
        pd::critical_error("Image Load Error", ss.str());
    }

    int x, y, comp;
    unsigned char *data = stbi_load_from_file(file, &x, &y, &comp, 0);
    fclose(file);

    SDL_Surface *rv;
    uint32_t rmask, gmask, bmask, amask;

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    rmask = 0xff000000;
    gmask = 0x00ff0000;
    bmask = 0x0000ff00;
    amask = 0x000000ff;
#else
    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = 0xff000000;
#endif

    if (comp == 4) {
        rv = SDL_CreateRGBSurface(0, x, y, 32, rmask, gmask, bmask, amask);
    } else if (comp == 3) {
        amask = 0;
        rv = SDL_CreateRGBSurface(0, x, y, 24, rmask, gmask, bmask, amask);
    } else {
        std::stringstream ss;
        ss << "Invalid image format for '" << filename <<
            "'.  We currently require 24bit or 32bit images, got " <<
            (comp * 8) << "bit";
        pd::critical_error("Image Load Error", ss.str());
    }

    memcpy(rv->pixels, data, comp * x * y);
    stbi_image_free(data);

    return rv;
}
