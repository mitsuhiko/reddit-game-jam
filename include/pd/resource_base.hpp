#ifndef _INC_PD_RESOURCE_BASE_HPP_
#define _INC_PD_RESOURCE_BASE_HPP_

#include <pd/pd.hpp>

namespace pd {

    class resource_manager;

    class resource_base {
    public:
        virtual ~resource_base() {}
        bool managed() const { return m_resmgr != 0; }
        pd::resource_manager *resmgr() { return m_resmgr; }
        const pd::resource_manager *resmgr() const { return m_resmgr; }

    private:
        friend class resource_manager;
        pd::resource_manager *m_resmgr;
    };
}

#endif
