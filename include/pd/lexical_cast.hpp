#ifndef _INC_PD_LEXICAL_CAST_HPP_
#define _INC_PD_LEXICAL_CAST_HPP_
#include <pd/pd.hpp>
#include <sstream>

namespace pd {

    template <class T>
    T lexical_cast(const std::string &value, bool &ok)
    {
        std::stringstream ss(value);
        T rv = T();
        ss >> std::skipws >> rv >> std::ws;
        ok = !ss.fail() && ss.eof();
        return rv;
    }

    template <class T>
    T lexical_cast(const std::string &value)
    {
        bool ok;
        return lexical_cast<T>(value, ok);
    }

    template <class T>
    std::string lexical_cast(const T &value)
    {
        std::stringstream ss;
        ss << value;
        return ss.str();
    }
}

#endif
