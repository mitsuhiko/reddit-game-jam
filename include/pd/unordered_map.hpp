#ifndef _INC_PD_UNORDERED_MAP_HPP_
#define _INC_PD_UNORDERED_MAP_HPP_

#include <pd/pd.hpp>

#if PD_PLATFORM == PD_PLATFORM_WINDOWS
#include <unordered_map>
#else
#include <tr1/unordered_map>
#endif

namespace pd {

    template <class Key, class Value>
    struct unordered_map {
#if PD_PLATFORM == PD_PLATFORM_WINDOWS
        typedef std::unordered_map<Key, Value> type;
#else
        typedef std::tr1::unordered_map<Key, Value> type;
#endif
    };
}

#endif
