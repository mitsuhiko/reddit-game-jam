#include <pd/main_menu.hpp>
#include <pd/game.hpp>
#include <pd/texture.hpp>
#include <pd/font.hpp>
#include <pd/drawtools.hpp>

pd::main_menu *pd::main_menu::s_instance;


pd::main_menu::main_menu()
{
    m_logo = pd::get_resource<pd::texture>("textures/logo.png");
    m_font = pd::get_resource<pd::bitmap_font>("fonts/simple.fnt");
}

pd::main_menu *pd::main_menu::instance()
{
    if (!s_instance)
        s_instance = new pd::main_menu();
    return s_instance;
}

void pd::main_menu::update(float dt)
{
}

void pd::main_menu::handle_event(SDL_Event &evt, float dt)
{
}

void pd::main_menu::render(float dt) const
{
    pd::clear_screen(0x333333ff);
    pd::draw_textured_quad(280.0f, 50.0f, m_logo);
    pd::draw_text("New Game", 600.0f, 280.0f, m_font);
    pd::draw_text("About", 600.0f, 310.0f, m_font);
    pd::draw_text("Quit", 600.0f, 340.0f, m_font);
}