#include <pd/console.hpp>
#include <pd/game.hpp>
#include <pd/font.hpp>
#include <pd/drawtools.hpp>
#include <pd/config.hpp>

pd::config::_console &cfg = pd::config::console;


pd::console::console()
{
    m_font = pd::get_resource<pd::bitmap_font>("fonts/console.fnt");
    m_visible = false;
}

void pd::console::write(const std::string &text)
{
    std::string full_text = m_line_buffer + text;
    m_line_buffer = "";

    size_t pos = 0;
    size_t end = full_text.size();

    while (pos < end) {
        size_t nl = full_text.find_first_of("\n", pos);
        std::string line = full_text.substr(pos, nl);
        if (nl == std::string::npos) {
            m_line_buffer = line;
            break;
        }
        m_lines.push_back(line);
        if (cfg.print_to_stderr)
            std::clog << "CONSOLE: " << line << std::endl;
        pos = nl + 1;
    }

    while ((int)m_lines.size() > cfg.max_lines)
        m_lines.pop_front();
}

void pd::console::write_line(const std::string &line)
{
    write(line);
    write("\n");
}

void pd::console::draw() const
{
    static const float x = 10.0f;
    float y = 10.0f;

    pd::draw_solid_quad(pd::vec2(0.0f, 0.0f), pd::game::instance().width(),
                        m_lines.size() * m_font->height() + 2.0f * y,
                        0x092C36EE);

    for (std::deque<std::string>::const_iterator iter = m_lines.begin();
         iter != m_lines.end(); ++iter) {
        pd::draw_text(*iter, pd::vec2(x, y), m_font);
        y += m_font->height();
    }

    if (!m_line_buffer.empty())
        pd::draw_text(m_line_buffer, pd::vec2(x, y), m_font);
}
