#include <pd/game_session.hpp>
#include <pd/game.hpp>
#include <pd/main_menu.hpp>
#include <pd/drawtools.hpp>
#include <pd/texture.hpp>
#include <pd/player.hpp>

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
        3, 5, 216, 41, 222, 11, 395, 34, 30, 8);
    m_electromagnetic_energy_bar = new pd::game_power_bar(interface_texture,
        3, 46, 216, 83, 222, 53, 395, 74, 30, 8);
    m_thermal_energy_bar = new pd::game_power_bar(interface_texture,
        5, 89, 216, 127, 222, 97, 395, 118, 30, 8);

    m_small_kinetic_energy_bar = new pd::game_power_bar(interface_texture,
        437, 13, 543, 31, 559, 13, 646, 23, 15, 5);
    m_small_electromagnetic_energy_bar = new pd::game_power_bar(interface_texture,
        437, 39, 543, 57, 559, 41, 646, 51, 15, 5);
    m_small_thermal_energy_bar = new pd::game_power_bar(interface_texture,
        437, 67, 543, 86, 559, 68, 646, 78, 15, 5);

    // create test environment
	m_map = pd::get_resource<pd::map>("maps/demo.map");
    m_player = new pd::player(40.0f, 300.0f);
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
    for (std::vector<pd::entity *>::iterator iter = m_entities.begin();
         iter != m_entities.end(); ++iter)
        (*iter)->update(dt);
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
	m_map->render();

    for (std::vector<pd::entity *>::const_iterator iter = m_entities.begin();
         iter != m_entities.end(); ++iter)
        (*iter)->render(dt);
    
    render_gui(dt);
}

void pd::game_session::render_gui(float dt) const
{
    float bary = 10.0f;

    #define DRAW_BAR(Name) do { \
        pd::game_power_bar *bar; \
        float xoff = 0.0f; \
        if (m_player->stance() == pd::player::Name##_stance) \
            bar = m_##Name##_energy_bar; \
        else { \
            bar = m_small_##Name##_energy_bar; \
            xoff = 20.0f; \
        } \
        bar->render(xoff + 10.0f, bary, m_player->Name##_energy()); \
        bary += bar->height() + 6.0f; \
    } while (0)

    DRAW_BAR(kinetic);
    DRAW_BAR(electromagnetic);
    DRAW_BAR(thermal);
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