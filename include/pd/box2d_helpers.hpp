#ifndef _INC_PD_BOX2D_HELPERS_HPP_
#define _INC_PD_BOX2D_HELPERS_HPP_
#include <pd/pd.hpp>

namespace pd {

    struct box2d_data_tuple {
        enum ptr_type {
            entity_type,
            block_type
        };

        ptr_type type;
        void *ptr;

        box2d_data_tuple(ptr_type type, void *ptr)
        {
            this->type = type;
            this->ptr = ptr;
        }
    };
}

#endif
