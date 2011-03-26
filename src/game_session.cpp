#include <pd/game_session.hpp>
#include <pd/game.hpp>
#include <pd/main_menu.hpp>
#include <pd/drawtools.hpp>


pd::game_session::game_session()
{
    pd::game::instance().resmgr().push();
	m_map = pd::get_resource<pd::map>("maps/demo.map");
}

pd::game_session::~game_session()
{
    pd::game::instance().resmgr().pop();
}

void pd::game_session::update(float dt)
{
}

void pd::game_session::handle_event(SDL_Event &evt, float dt)
{
    if (evt.type == SDL_KEYDOWN && evt.key.keysym.sym == SDLK_ESCAPE) {
        pd::game::instance().screen(pd::main_menu::instance());
        // suicide, because the main menu adds a new instance of us
        delete this;
    }
}

void pd::game_session::render(float dt) const
{
    pd::clear_screen(0x000000ff);
	m_map->render();
}