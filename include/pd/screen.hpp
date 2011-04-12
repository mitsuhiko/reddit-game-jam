#ifndef _INC_PD_SCREEN_HPP_
#define _INC_PD_SCREEN_HPP_
#include <pd/pd.hpp>

namespace pd {

    class screen {
    public:
        virtual ~screen() {}

        virtual void update(pd::timedelta_t dt);
        virtual void handle_event(SDL_Event &evt);
        virtual void render(pd::timedelta_t dt) const;
    };
}

#endif
