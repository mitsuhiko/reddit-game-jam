#include <pd/game_session.hpp>
#include <pd/game.hpp>
#include <pd/main_menu.hpp>
#include <pd/drawtools.hpp>
#include <pd/texture.hpp>
#include <pd/player.hpp>
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
            m_bar_offset = pd::vec2(box, boy);
        }

        ~game_power_bar()
        {
            delete m_panel;
            delete m_bar;
        }

        int height() const { return m_panel->height(); }

        void render(const pd::vec2 &pos, float value)
        {
            pd::draw_quad(m_panel, pos);
            pd::draw_quad(m_bar, pos + m_bar_offset,
                          m_bar->width() * value, (float)m_bar->height());
        }

    private:
        pd::texture *m_panel;
        pd::texture *m_bar;
        pd::vec2 m_bar_offset;
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
	m_map = new pd::map(this, "maps/level01.tmx");

    m_player = new pd::player(this, pd::vec2(400.0f, 0.0f));
    m_enemies.push_back(new pd::kinetic_enemy(this, pd::vec2(100.0f, 0.0f)));

    m_draw_bounds = false;
}

pd::game_session::~game_session()
{
    delete m_map;
    delete m_player;

    for (std::vector<pd::enemy *>::iterator iter = m_enemies.begin();
         iter != m_enemies.end(); ++iter)
        delete *iter;
    pd::game::instance().resmgr().pop();
}

void pd::game_session::update(pd::timedelta_t dt)
{
    m_player->update(dt);
    for (std::vector<pd::enemy *>::iterator iter = m_enemies.begin();
         iter != m_enemies.end(); ++iter)
        (*iter)->update(dt);

    m_cam->look_at(m_player->pos(), dt);
}

void pd::game_session::handle_event(SDL_Event &evt, pd::timedelta_t dt)
{
    if (evt.type == SDL_KEYDOWN) {
        switch (evt.key.keysym.sym) {
        case SDLK_ESCAPE:
            pd::game::instance().screen(pd::main_menu::instance());
            // suicide, because the main menu adds a new instance
            delete this;
            return;
        case SDLK_b:
            m_draw_bounds = !m_draw_bounds;
            return;
        }
    }

    m_player->handle_event(evt, dt);
    for (std::vector<pd::enemy *>::iterator iter = m_enemies.begin();
         iter != m_enemies.end(); ++iter)
        (*iter)->handle_event(evt, dt);
}

void pd::game_session::render(pd::timedelta_t dt) const
{
    pd::push_matrix();

    m_cam->apply();
	m_map->render();

    m_player->render(dt);

    for (std::vector<pd::enemy *>::const_iterator iter = m_enemies.begin();
         iter != m_enemies.end(); ++iter)
        (*iter)->render(dt);

    if (m_draw_bounds) {
        m_map->draw_tile_bounds();
        pd::draw_debug_box(m_player->bounding_box(), 0x336699ff);
        for (std::vector<pd::enemy *>::const_iterator iter = m_enemies.begin();
             iter != m_enemies.end(); ++iter)
            pd::draw_debug_box((*iter)->bounding_box(), 0xff0000ff);
    }

    pd::pop_matrix();
    
    render_gui(dt);
}

void pd::game_session::render_gui(pd::timedelta_t dt) const
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

    bar->render(pd::vec2(10.0f, 10.0f), m_player->energy());
}
