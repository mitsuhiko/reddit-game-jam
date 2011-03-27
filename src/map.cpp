#include <pd/map.hpp>
#include <pd/game.hpp>
#include <pd/game_session.hpp>
#include <pd/texture.hpp>
#include <pd/drawtools.hpp>
#include <pd/lexical_cast.hpp>
#include <sstream>
#include <fstream>


pd::map::map(pd::game_session *session, std::string filename)
{
    m_session = session;

    std::ifstream in(filename.c_str());
    if (!in.good()) {
        std::stringstream ss;
        ss << "Unable to load map from '" << filename << "'";
        pd::critical_error("Cannot load map", ss.str());
    }

    std::string tileset, width, height, tile_width, tile_height, background_color;
    std::getline(in, tileset, '\0');
    std::getline(in, width, '\0');
    std::getline(in, height, '\0');
    std::getline(in, tile_width, '\0');
    std::getline(in, tile_height, '\0');
    std::getline(in, background_color, '\0');
    m_width = pd::lexical_cast<int>(width);
    m_height = pd::lexical_cast<int>(height);
    m_tile_width = pd::lexical_cast<int>(tile_width);
    m_tile_height = pd::lexical_cast<int>(tile_height);
    m_background_color = pd::color(background_color);

    m_background = new tile_id_t[m_width * m_height];
    m_foreground = new tile_id_t[m_width * m_height];
    m_blocks = new pd::block*[m_width * m_height];

    in.read((char *)m_background, m_width * m_height);
    in.read((char *)m_foreground, m_width * m_height);

    m_tileset = pd::get_resource<pd::texture>(tileset);
    tile_id_t i = 1;
    for (int y = 0; y < m_tileset->height(); y += m_tile_height) {
        for (int x = 0; x < m_tileset->width(); x += m_tile_width) {
            pd::texture *tile = m_tileset->slice(x, y, m_tile_width, m_tile_height);
            m_tiles[i++] = tile;
        }
    }

    for (int y = 0; y < m_height; y++) {
        for (int x = 0; x < m_width; x++) {
            tile_id_t tile = get_fg(x, y);
            pd::block *block = try_make_block(tile, x, y);
            if (block)
                m_foreground[(y * m_width) + x] = 0;
            m_blocks[(y * m_width) + x] = block;
        }
    }
}

pd::map::~map()
{
    for (int i = 0; i < (m_width * m_height); i++)
        delete m_blocks[i];

    delete[] m_background;
    delete[] m_foreground;
    delete[] m_blocks;
}

pd::block *pd::map::try_make_block(tile_id_t tile, int x, int y)
{
    pd::block::block_type type;
    switch (tile) {
    case 129:
        type = pd::block::metal_type;
        break;
    case 130:
        type = pd::block::ice_type;
        break;
    case 131:
        type = pd::block::glass_type;
        break;
    case 132:
        type = pd::block::lava_type;
        break;
    case 133:
        type = pd::block::glass_type;
        break;
    default:
        return 0;
    }

    std::map<tile_id_t, pd::texture *>::iterator iter = m_tiles.find(tile);
    assert(iter != m_tiles.end());

    return new pd::block(this, iter->second, type, x, y);
}

void pd::map::draw_tile(int x, int y, pd::map::tile_id_t tile) const
{
    if (!tile)
        return;
    std::map<tile_id_t, pd::texture *>::const_iterator iter = m_tiles.find(tile);
    assert(iter != m_tiles.end());
    pd::draw_textured_quad((float)x * m_tile_width,
                           (float)y * m_tile_height, iter->second);
}

void pd::map::render() const
{
    pd::clear_screen(m_background_color);
    for (int y = 0; y < m_height; y++) {
        for (int x = 0; x < m_width; x++) {
            draw_tile(x, y, get_bg(x, y));
            draw_tile(x, y, get_fg(x, y));
            const pd::block *block = get_block(x, y);
            if (block)
                block->render();
        }
    }
}

pd::block::block(pd::map *map, pd::texture *texture, block_type type, int x, int y)
{
    m_map = map;
    m_texture = texture;
    m_type = type;
    m_x = x;
    m_y = y;
}

void pd::block::render() const
{
    pd::draw_textured_quad((float)m_x * m_map->tile_width(),
                           (float)m_y * m_map->tile_height(), m_texture);
}