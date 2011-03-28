#include <pd/main_menu.hpp>
#include <pd/game.hpp>
#include <pd/texture.hpp>
#include <pd/font.hpp>
#include <pd/drawtools.hpp>
#include <pd/game_session.hpp>

pd::main_menu *pd::main_menu::s_instance;

namespace {

    struct menu_item {
        const char *caption;
        void (*callback)();
    };

    void on_new_game()
    {
        pd::game::instance().screen(new pd::game_session());
    }

    void on_quit()
    {
        pd::game::instance().stop();
    }

    static menu_item items[] = {
        {"New Game", on_new_game},
        {"About", 0},
        {"Quit", on_quit}
    };
    static const int item_count = (sizeof(items) / sizeof(items[0]));
}


pd::main_menu::main_menu()
{
    m_logo = pd::get_resource<pd::texture>("textures/logo.png");
    m_font = pd::get_resource<pd::bitmap_font>("fonts/simple.fnt");
    m_active_item = 0;
}

pd::main_menu *pd::main_menu::instance()
{
    if (!s_instance)
        s_instance = new pd::main_menu();
    return s_instance;
}

void pd::main_menu::update(pd::timedelta_t dt)
{
}

void pd::main_menu::handle_event(SDL_Event &evt, pd::timedelta_t dt)
{
    if (evt.type == SDL_KEYDOWN) {
        switch (evt.key.keysym.sym) {
        case SDLK_UP:
        case SDLK_w:
            if (m_active_item > 0)
                m_active_item--;
            break;
        case SDLK_DOWN:
        case SDLK_s:
            if (m_active_item < item_count - 1)
                m_active_item++;
            break;
        case SDLK_SPACE:
        case SDLK_RETURN:
            if (items[m_active_item].callback)
                items[m_active_item].callback();
        }
    }
}

void pd::main_menu::render(pd::timedelta_t dt) const
{
    pd::clear_screen(0x333333ff);
    pd::reset_color();
    pd::draw_textured_quad(glm::vec2(280.0f, 50.0f), m_logo);

    for (int i = 0; i < item_count; i++) {
        pd::set_color(i == m_active_item ? 0x59b1d2ff : 0xffffffff);
        pd::draw_text(items[i].caption, glm::vec2(600.0f, 280.0f + (i * 30.0f)), m_font);
    }
}