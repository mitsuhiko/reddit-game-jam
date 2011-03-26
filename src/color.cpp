#include <pd/color.hpp>


pd::color::color(const std::string &value)
{
    const char *str = value.c_str();
    if (str[0] == '#')
        str++;
    unsigned int rv;
    sscanf(str, "%x", &rv);
    r = rv >> 16;
    g = rv >> 8 & 0xff;
    b = rv & 0xff;
    a = 0xff;
}