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

    // XXX: endianess
    SDL_Surface *rv = SDL_CreateRGBSurface(0, x, y, 32, 0xff,
        0xff00, 0xff0000, 0xff000000);

    if (comp == 4) {
        memcpy(rv->pixels, data, 32 * x * y);
    } else {
        std::stringstream ss;
        ss << "Invalid image format for '" << filename << "'.  Need 32bit";
        pd::critical_error("Image Load Error", ss.str());
    }

    stbi_image_free(data);

    return 0;
}