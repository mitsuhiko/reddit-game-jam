#include <pd/map.hpp>
#include <pd/game.hpp>
#include <pd/game_session.hpp>
#include <pd/texture.hpp>
#include <pd/drawtools.hpp>
#include <pd/lexical_cast.hpp>
#include <sstream>
#include <fstream>


pd::block::block(pd::map *map, block_type type, float x, float y)
{
    m_map = map;
    m_type = type;

    b2BodyDef bodydef;
    bodydef.type = b2_dynamicBody;
    bodydef.position.Set(pd::pixel_to_meter(x), pd::pixel_to_meter(y));
    b2Body *body = map->session()->box2d_world()->CreateBody(&bodydef);
    b2FixtureDef fixturedef;
    b2PolygonShape fixedbox;
    fixedbox.SetAsBox(pd::pixel_to_meter(map->tile_width() / 2.0f),
                      pd::pixel_to_meter(map->tile_height() / 2.0f));
    fixturedef.shape = &fixedbox;
    fixturedef.density = 0;
    fixturedef.friction = 1.5f;
    m_fixture = body->CreateFixture(&fixturedef);
    m_body = body;
}

pd::block::~block()
{
    m_map->session()->box2d_world()->DestroyBody(m_body);
}


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
        int row_start = -1;
        for (int x = 0; x < m_width; x++) {
            tile_id_t tile = get_fg(x, y);
            pd::block *block = try_make_block(tile, x, y);
            if (block) {
                m_foreground[(y * m_width) + x] = tile = 0;
                m_blocks.push_back(block);
            }
            if (row_start < 0) {
                if (tile)
                    row_start = x;
                continue;
            }
            if (tile)
                continue;
            create_ground_box(row_start, y, x - row_start);
            row_start = -1;
        }

        if (row_start >= 0)
            create_ground_box(row_start, y, m_width - 1);
    }
}

pd::map::~map()
{
    delete[] m_background;
    delete[] m_foreground;
}

pd::block *pd::map::try_make_block(tile_id_t tile, int x, int y)
{
    /* TODO: detect block and create one */
    return 0;
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
        }
    }
}

void pd::map::create_ground_box(int x, int y, int length)
{
    b2BodyDef bodydef;
    bodydef.type = b2_staticBody;
    bodydef.position.Set(pd::pixel_to_meter((float)m_tile_width * (x + length / 2.0f)),
                         pd::pixel_to_meter((float)m_tile_height * y));
    bodydef.fixedRotation = true;
    b2Body *body = m_session->box2d_world()->CreateBody(&bodydef);
    b2FixtureDef fixturedef;
    b2PolygonShape fixedbox;
    fixedbox.SetAsBox(pd::pixel_to_meter(m_tile_width * length / 2.0f),
                      pd::pixel_to_meter(m_tile_height / 2.0f));
    fixturedef.shape = &fixedbox;
    fixturedef.density = 0;
    fixturedef.friction = 1.5f;
    body->CreateFixture(&fixturedef);
}