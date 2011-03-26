#include <pd/font.hpp>
#include <pd/texture.hpp>
#include <pd/lexical_cast.hpp>
#include <pd/path.hpp>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iterator>


namespace {
    std::vector<std::string> split_string(const std::string &str)
    {
        std::vector<std::string> rv;
        std::stringstream ss(str);
        std::copy(std::istream_iterator<std::string>(ss),
             std::istream_iterator<std::string>(),
             std::back_inserter<std::vector<std::string> >(rv));
        return rv;
    }

    template <class Iterator>
    std::map<std::string, std::string> kv_split(Iterator iter, Iterator end)
    {
        std::map<std::string, std::string> rv;
        
        for (; iter != end; ++iter) {
            const std::string &tok = *iter;
            size_t split = tok.find_first_of('=');
            if (split != std::string::npos)
                rv[tok.substr(0, split)] = tok.substr(split + 1);
        }

        return rv;
    }

    std::string unquote(const std::string &val)
    {
        if (val.empty())
            return val;
        if (val[0] == '"' && val[val.size() - 1] == '"')
            return val.substr(1, val.size() - 2);
        return val;
    }
}


pd::bitmap_font::bitmap_font(const std::string &filename)
: m_dummy_char(0, 0, 0, 0)
{
    std::ifstream in(filename.c_str());
    if (!in.good()) {
        std::stringstream ss;
        ss << "Font '" << filename << "' was not found";
        pd::critical_error("Could not load font", ss.str());
    }

    m_texture = 0;
    m_height = 0;
    std::string line;
    while (!in.eof()) {
        std::getline(in, line);
        std::vector<std::string> tokens(split_string(line));
        if (tokens.empty())
            continue;
        std::map<std::string, std::string> opts = kv_split(tokens.begin() + 1, tokens.end());
        if (tokens[0] == "common") {
            m_height = pd::lexical_cast<int>(opts["lineHeight"]);
        } else if (tokens[0] == "page") {
            if (m_texture)
                pd::critical_error("Font format error", "Font texture specified twice");
            m_texture = pd::load_texture(pd::path::join_with_dir(filename, unquote(opts["file"])));
        } else if (tokens[0] == "char") {
            if (!m_texture)
                pd::critical_error("Font format error", "Char def before font texture def");
            unsigned char c = (unsigned char)pd::lexical_cast<int>(opts["id"]);
            pd::texture *texture = m_texture->slice(
                pd::lexical_cast<int>(opts["x"]),
                pd::lexical_cast<int>(opts["y"]),
                pd::lexical_cast<int>(opts["width"]),
                pd::lexical_cast<int>(opts["height"])
            );
            m_glyphs.insert(std::make_pair(c, pd::glyph_info(
                pd::lexical_cast<int>(opts["xadvance"]),
                pd::lexical_cast<int>(opts["xoffset"]),
                pd::lexical_cast<int>(opts["yoffset"]),
                texture
            )));
        }
    }
}

const pd::glyph_info &pd::bitmap_font::get(unsigned char c) const
{
    std::map<unsigned char, glyph_info>::const_iterator iter = m_glyphs.find(c);
    if (iter == m_glyphs.end())
        return m_dummy_char;
    return iter->second;
}

pd::bitmap_font::~bitmap_font()
{
    std::map<unsigned char, glyph_info>::iterator iter;
    for (iter = m_glyphs.begin(); iter != m_glyphs.end(); ++iter)
        delete iter->second.texture;
    delete m_texture;
}
