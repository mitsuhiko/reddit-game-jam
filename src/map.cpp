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
	for (int x = 0; x != m_tile_width; x++) {
		delete[] m_background[x];
		delete[] m_foreground[x];
	}
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
	m_background = new tile_id_t*[m_width];
	m_foreground = new tile_id_t*[m_width];

	for (int x = 0; x != m_tile_width; x++) {
		m_background[x] = new tile_id_t[m_height];
        fread(m_background[x], 1, m_width, fp);
	}
	for (int x = 0; x != m_tile_width; x++) {
		m_foreground[x] = new tile_id_t[m_height];
		fread(m_foreground[x], 1, m_width, fp);
	}

	m_tileset = pd::get_resource<pd::texture>(tileset);
	printf("Read map %s, with tileset %s, size %ix%i and tile size %ix%i.\n",
		filename.c_str(), tileset, m_width, m_height,
		m_tile_width, m_tile_height);

	fclose(fp);
	return true;
}

void pd::map::render()
{
	pd::draw_textured_quad(20, 20, m_tileset);
}
