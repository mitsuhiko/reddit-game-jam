#ifndef _INC_PD_UNORDERED_MAP_HPP_
#define _INC_PD_UNORDERED_MAP_HPP_
#include <pd/pd.hpp>

#ifdef _MSC_VER
#include <unordered_map>
#else
#include <tr1/unordered_map>
#endif


namespace pd {

    using std::tr1::unordered_map;
    using std::tr1::unordered_multimap;
}

#endif
