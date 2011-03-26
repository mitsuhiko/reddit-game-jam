#pragma once
#include <string>
#include <pd/texture.hpp>
namespace pd {
class map
{
public:
	int width;
	int height;
	int tile_width;
	int tile_height;
	char** background;
	char** foreground;
	pd::texture *text;

	map(std::string filename);
	bool load(std::string filename);
	void render();
	~map(void);
};
}