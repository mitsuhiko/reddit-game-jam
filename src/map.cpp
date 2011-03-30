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
    m_blocks = new pd::block*[m_width * m_height];

    in.read((char *)m_background, m_width * m_height);

    tile_id_t *foreground = new tile_id_t[m_width * m_height];
    in.read((char *)foreground, m_width * m_height);

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
            tile_id_t tile = foreground[(y * m_width) + x];
            pd::block *block = tile ? new pd::block(this, tile, x, y) : 0;
            m_blocks[(y * m_width) + x] = block;
        }
    }
 
    delete[] foreground;
}

pd::map::~map()
{
    for (int i = 0; i < (m_width * m_height); i++)
        delete m_blocks[i];

    delete[] m_background;
    delete[] m_blocks;
}

pd::collision_flag pd::map::get_collision(int x, int y) const
{
    const pd::block *block = get_block(x, y);
    if (!block)
        return passable;
    return block->collision();
}

void pd::map::render_tile(int x, int y, pd::map::tile_id_t tile) const
{
    if (!tile)
        return;
    std::map<tile_id_t, pd::texture *>::const_iterator iter = m_tiles.find(tile);
    assert(iter != m_tiles.end());
    pd::draw_textured_quad(glm::vec2((float)x * m_tile_width,
                                     (float)y * m_tile_height), iter->second);
}

void pd::map::render() const
{
    pd::clear_screen(m_background_color);
    for (int y = 0; y < m_height; y++) {
        for (int x = 0; x < m_width; x++) {
            render_tile(x, y, get_bg(x, y));
            const pd::block *block = get_block(x, y);
            if (block)
                render_tile(x, y, block->tile());
        }
    }
}

pd::block::block(pd::map *map, pd::map::tile_id_t tile, int x, int y)
{
    m_map = map;
    m_tile = tile;
    m_x = x;
    m_y = y;
}

pd::aabb pd::block::bounding_box() const
{
    glm::vec2 pos(m_map->tile_width() * m_x, m_map->tile_height() * m_y);
    glm::vec2 size(m_map->tile_width(), m_map->tile_height());
    return pd::aabb(pos, pos + size);
}

pd::collision_flag pd::block::collision() const
{
    if (m_tile == 0)
        return pd::passable;
    return pd::impassable;
}
