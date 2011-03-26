#include <pd/map.hpp>
#include <stdio.h>
namespace pd {
pd::map::map(std::string filename)
{
	this->load(filename);
}


pd::map::~map(void)
{
}

bool pd::map::load( std::string filename )
{
	/*
	std::ifstream input;
	input.open(filename.c_str(), std::ios::in);
	std::string tileset;
	std::string width;
	std::string height;
	std::string tileset_width;
	std::string tileset_height;

	input >> tileset;
	input >> width;
	input >> height;
	input >> tileset_width;
	input >> tileset_height;

	this->width = atoi(width.c_str());
	this->height = atoi(height.c_str());
	//text = pd::load_texture(tileset);
	
	input.close();
	return true;
	*/

	FILE* fp;
	fp = fopen(filename.c_str(), "r");
	if (fp == NULL) {
		printf("Unable to load map located in %s.", filename.c_str());
	}
	char tileset[512], width[16], height[16], tile_width[16], tile_height[16]; // Good enough for the moment.
	fscanf(fp, "%s%s%s%s%s", tileset, width, height, tile_width, tile_height);
	this->width = atoi(width);
	this->height = atoi(height);
	this->tile_width = atoi(tile_width);
	this->tile_height = atoi(tile_height);
	this->background = (char**)malloc(this->width);
	this->foreground = (char**)malloc(this->width);
	for (int x = 0; x != this->tile_width; x++) {
		this->background[x] = (char*)malloc(this->height);
		fread(this->background[x], 1, this->width, fp);
	}
	for (int x = 0; x != this->tile_width; x++) {
		this->foreground[x] = (char*)malloc(this->height);
		fread(this->foreground[x], 1, this->width, fp);
	}

	printf("Read map %s, with tileset %s, size %ix%i and tile size %ix%i\n.",
		filename.c_str(), tileset, this->width, this->height,
		this->tile_width, this->tile_height);

	fclose(fp);
	return true;
}

void map::render()
{

}
}