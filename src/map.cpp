#include <pd/map.hpp>
#include <stdio.h>

pd::map::map(std::string filename)
{
	load(filename);
}

pd::map::~map(void)
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
	if (fp == NULL) {
		printf("Unable to load map located in %s.", filename.c_str());
	}
	char tileset[512], width[16], height[16], tile_width[16], tile_height[16]; // Good enough for the moment.
	fscanf(fp, "%s%s%s%s%s", tileset, width, height, tile_width, tile_height);
	m_width = atoi(width);
	m_height = atoi(height);
	m_tile_width = atoi(tile_width);
	m_tile_height = atoi(tile_height);
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

	printf("Read map %s, with tileset %s, size %ix%i and tile size %ix%i\n.",
		filename.c_str(), tileset, m_width, m_height,
		m_tile_width, m_tile_height);

	fclose(fp);
	return true;
}

void pd::map::render()
{

}
