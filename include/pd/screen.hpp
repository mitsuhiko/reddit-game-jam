#ifndef _INC_PD_SCREEN_HPP_
#define _INC_PD_SCREEN_HPP_
#include <pd/pd.hpp>

namespace pd {

    class screen {
    public:
        virtual ~screen() {}

        virtual void update(float dt);
        virtual void handle_event(SDL_Event &evt, float dt);
        virtual void render(float dt) const;
    };
}

#endif
