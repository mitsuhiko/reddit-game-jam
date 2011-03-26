#ifndef map_h_guard
#define map_h_guard

#include <string>
#include <pd/texture.hpp>
#include <pd/resource_base.hpp>

namespace pd {
class map : resource_base
{
public:
	int m_width;
	int m_height;
	int m_tile_width;
	int m_tile_height;
	char** m_background;
	char** m_foreground;
	pd::texture *m_text;

	map(std::string filename);
	bool load(std::string filename);
	void render();
	~map(void);
};
}
#endif // map_h_guard