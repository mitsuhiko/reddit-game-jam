#include <pd/map.hpp>
#include <pd/game.hpp>
#include <pd/texture.hpp>
#include <pd/drawtools.hpp>
#include <pd/lexical_cast.hpp>
#include <sstream>
#include <cstdio>

pd::map::map(std::string filename)
{
    load(filename);
}

pd::map::~map()
{
    delete[] m_background;
    delete[] m_foreground;
}

/* Loads map and loads texture from map file. */
bool pd::map::load(std::string filename)
{
    FILE* fp;
    fp = fopen(filename.c_str(), "r");
    if (!fp) {
        std::stringstream ss;
        ss << "Unable to load map from '" << filename << "'";
        pd::critical_error("Cannot load map", ss.str());
    }

    // Good enough for the moment.
    char tileset[512], width[16], height[16], tile_width[16], tile_height[16];
    fscanf(fp, "%s%s%s%s%s", tileset, width, height, tile_width, tile_height);
    m_width = pd::lexical_cast<int>(width);
    m_height = pd::lexical_cast<int>(height);
    m_tile_width = pd::lexical_cast<int>(tile_width);
    m_tile_height = pd::lexical_cast<int>(tile_height);
    m_background = new tile_id_t[m_width * m_height];
    m_foreground = new tile_id_t[m_width * m_height];

    fread(m_background, 1, m_width * m_height, fp);
    fread(m_foreground, 1, m_width * m_height, fp);

    m_tileset = pd::get_resource<pd::texture>(tileset);

    tile_id_t i = 1;
    for (int y = 0; y < m_tileset->height(); y += m_tile_height) {
        for (int x = 0; x < m_tileset->width(); x += m_tile_width) {
            pd::texture *tile = m_tileset->slice(x, y, m_tile_width, m_tile_height);
            m_tiles[i++] = tile;
        }
    }

    printf("Read map %s, with tileset %s, size %ix%i and tile size %ix%i.\n",
        filename.c_str(), tileset, m_width, m_height,
        m_tile_width, m_tile_height);

    fclose(fp);
    return true;
}

void pd::map::render()
{
    for (int x = 0; x < m_width; x++) {
        for (int y = 0; y < m_height; y++) {
            tile_id_t tile = get_bg(x, y);
            if (!tile)
                continue;
            std::map<tile_id_t, pd::texture *>::iterator iter = m_tiles.find(tile);
            assert(iter != m_tiles.end());
            pd::draw_textured_quad(x * m_tile_width, y * m_tile_height, iter->second);
        }
    }
}
