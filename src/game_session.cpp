#include <pd/game_session.hpp>
#include <pd/game.hpp>
#include <pd/main_menu.hpp>
#include <pd/drawtools.hpp>
#include <pd/player.hpp>


pd::game_session::game_session()
{
    pd::game::instance().resmgr().push();
    m_world = new b2World(b2Vec2(0, 9.79), true);
	m_map = pd::get_resource<pd::map>("maps/demo.map");

    m_player = new pd::player(m_world, 40.0f, 300.0f);
    add_entity(m_player);
}

pd::game_session::~game_session()
{
    for (std::vector<pd::entity *>::const_iterator iter = m_entities.begin();
         iter != m_entities.end(); ++iter)
        delete *iter;
    pd::game::instance().resmgr().pop();
}

void pd::game_session::update(float dt)
{
    m_world->Step(dt, 10, 10);
    for (std::vector<pd::entity *>::iterator iter = m_entities.begin();
         iter != m_entities.end(); ++iter)
        (*iter)->update(dt);
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
	m_map->render();

    for (std::vector<pd::entity *>::const_iterator iter = m_entities.begin();
         iter != m_entities.end(); ++iter)
        (*iter)->render(dt);
}

void pd::game_session::add_entity(pd::entity *entity)
{
    m_entities.push_back(entity);
}

bool pd::game_session::remove_entity(pd::entity *entity)
{
    std::vector<pd::entity *>::iterator iter;
    for (iter = m_entities.begin(); iter != m_entities.end(); ++iter)
        if (*iter == entity) {
            delete entity;
            m_entities.erase(iter);
            return true;
        }
    return false;
}