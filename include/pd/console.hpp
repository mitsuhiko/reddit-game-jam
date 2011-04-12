#ifndef _INC_PD_CONSOLE_HPP_
#define _INC_PD_CONSOLE_HPP_
#include <pd/pd.hpp>
#include <pd/game.hpp>
#include <deque>
#include <sstream>


#define PD_CLOG(Expr) do { \
    std::stringstream ss; ss << Expr; \
    pd::game::instance().console().write_line(ss.str()); \
} while (0)


namespace pd {

    class font;

    class console {
    public:
        console();

        void write(const std::string &text);
        void write_line(const std::string &line);
        void draw() const;

        bool visible() const { return m_visible; }
        void visible(bool val) { m_visible = val; }
        void toggle_visibility() { m_visible = !m_visible; }

    private:
        std::string m_line_buffer;
        std::deque<std::string> m_lines;
        bool m_visible;

        pd::font *m_font;
    };
}

#endif
