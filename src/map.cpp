#include <pd/map.hpp>
#include <pd/game.hpp>
#include <pd/texture.hpp>
#include <pd/drawtools.hpp>
#include <pd/lexical_cast.hpp>
#include <sstream>
#include <fstream>


pd::map::map(std::string filename)
{
    m_world = NULL;
    load(filename);
}

pd::map::~map()
{
    delete[] m_background;
    delete[] m_foreground;
}

/* Loads map and loads texture from map file. */
void pd::map::load(std::string filename)
{
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

void pd::map::create_ground_box(float x, float y, float width)
{
    assert(m_world != NULL);
    b2BodyDef bodydef;
    bodydef.type = b2_staticBody;
    bodydef.position.Set(pd::pixel_to_meter(x), pd::pixel_to_meter(y));
    bodydef.fixedRotation = true;
    b2Body *body = m_world->CreateBody(&bodydef);
    b2FixtureDef fixturedef;
    b2PolygonShape fixedbox;
    fixedbox.SetAsBox(pd::pixel_to_meter(width / 2.0f),
                      pd::pixel_to_meter(m_tile_height / 2.0f));
    fixturedef.shape = &fixedbox;
    fixturedef.density = 0;
    fixturedef.friction = 0.5f;
    body->CreateFixture(&fixturedef);
}

/* Builds a collision mask using RLE. */
void pd::map::build_box2d_object()
{
    create_ground_box(0, 300, 1500);
    tile_id_t last_tile = 0;
    for (int x = 0; x < m_width;  x++) {
        for (int y = 0; y < m_height; y++) {
            if (!last_tile) {
            }

        }
    }
}