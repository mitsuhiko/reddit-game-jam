#include <pd/game_session.hpp>
#include <pd/game.hpp>
#include <pd/main_menu.hpp>
#include <pd/drawtools.hpp>
#include <pd/texture.hpp>
#include <pd/player.hpp>
#include <pd/thermal_enemy.hpp>
#include <pd/kinetic_enemy.hpp>
#include <pd/camera.hpp>

namespace pd {

    class game_power_bar {
    public:
        game_power_bar(pd::texture *tex, int pbx1, int pby1, int pbx2, int pby2,
                       int bbx1, int bby1, int bbx2, int bby2, int box, int boy)
        {
            m_panel = tex->slice(pbx1, pby1, pbx2 - pbx1, pby2 - pby1);
            m_bar = tex->slice(bbx1, bby1, bbx2 - bbx1, bby2 - bby1);
            m_bar_off_x = box;
            m_bar_off_y = boy;
        }

        ~game_power_bar()
        {
            delete m_panel;
            delete m_bar;
        }

        int height() const { return m_panel->height(); }

        void render(float x, float y, float value)
        {
            pd::draw_textured_quad(x, y, m_panel);
            pd::draw_textured_quad(x + m_bar_off_x, y + m_bar_off_y,
                m_bar->width() * value, (float)m_bar->height(), m_bar);
        }

    private:
        pd::texture *m_panel;
        pd::texture *m_bar;
        int m_bar_off_x;
        int m_bar_off_y;
    };
}


pd::game_session::game_session()
{
    pd::game::instance().resmgr().push();

    pd::texture *interface_texture = pd::get_resource<pd::texture>("textures/interface.png");
    m_kinetic_energy_bar = new pd::game_power_bar(interface_texture,
        3, 5, 216, 41, 222, 11, 395, 32, 30, 8);
    m_electromagnetic_energy_bar = new pd::game_power_bar(interface_texture,
        3, 46, 216, 83, 222, 53, 395, 74, 30, 8);
    m_thermal_energy_bar = new pd::game_power_bar(interface_texture,
        5, 89, 216, 127, 222, 97, 395, 118, 30, 8);

    m_cam = new pd::camera();

    // create test environment
    m_world = new b2World(b2Vec2(0, 9.79f), true);
	m_map = new pd::map(this, "maps/testlevel.map");

    m_player = new pd::player(this, 400.0f, 200.0f);
    //new pd::thermal_enemy(this, 240.0f, 0.0f);
    new pd::kinetic_enemy(this, 15.0f, 0.0f);
}

pd::game_session::~game_session()
{
    delete m_map;

    for (std::vector<pd::entity *>::const_iterator iter = m_entities.begin();
         iter != m_entities.end(); ++iter)
        delete *iter;
    pd::game::instance().resmgr().pop();
}

void pd::game_session::update(float dt)
{
    m_world->Step(dt, 10, 10);

    std::vector<pd::entity *> dead_entities;
    for (std::vector<pd::entity *>::iterator iter = m_entities.begin();
         iter != m_entities.end(); ++iter) {
        if ((*iter)->dead())
            dead_entities.push_back(*iter);
        else
            (*iter)->update(dt);
    }

    uint8_t *state = SDL_GetKeyboardState(NULL);
    if (state[SDL_SCANCODE_D] || state[SDL_SCANCODE_RIGHT]) {
        m_player->move_right();
    } else if (state[SDL_SCANCODE_A] || state[SDL_SCANCODE_LEFT]) {
        m_player->move_left();
    } else {
        m_player->stop();
    }
    m_player->shooting(state[SDL_SCANCODE_SPACE] != 0);

    m_cam->look_at(m_player->x(), m_player->y(), dt);

    for (std::vector<pd::entity *>::iterator iter = dead_entities.begin();
         iter != dead_entities.end(); ++iter)
        remove_entity(*iter);
}

void pd::game_session::handle_event(SDL_Event &evt, float dt)
{
    if (evt.type == SDL_KEYDOWN) {
        switch (evt.key.keysym.sym) {
        case SDLK_ESCAPE:
            pd::game::instance().screen(pd::main_menu::instance());
            // suicide, because the main menu adds a new instance of us
            delete this;
            break;
        case SDLK_w:
            m_player->jump();
            break;
        case SDLK_1:
            m_player->stance(pd::player::kinetic_stance);
            break;
        case SDLK_2:
            m_player->stance(pd::player::electromagnetic_stance);
            break;
        case SDLK_3:
            m_player->stance(pd::player::thermal_stance);
            break;
        }
    }
}

void pd::game_session::render(float dt) const
{
    pd::push_matrix();

    m_cam->apply();
	m_map->render();

    for (std::vector<pd::entity *>::const_iterator iter = m_entities.begin();
         iter != m_entities.end(); ++iter)
        (*iter)->render(dt);

    pd::pop_matrix();
    
    render_gui(dt);
}

void pd::game_session::render_gui(float dt) const
{
    pd::game_power_bar *bar;
    switch (m_player->stance()) {
    case pd::player::kinetic_stance:
        bar = m_kinetic_energy_bar;
        break;
    case pd::player::electromagnetic_stance:
        bar = m_electromagnetic_energy_bar;
        break;
    case pd::player::thermal_stance:
        bar = m_thermal_energy_bar;
        break;
    default:
        assert(false);
    }

    bar->render(10.0f, 10.0f, m_player->energy());
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