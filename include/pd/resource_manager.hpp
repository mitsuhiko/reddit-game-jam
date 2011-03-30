#ifndef _INC_PD_RESOURCE_MANAGER_HPP_
#define _INC_PD_RESOURCE_MANAGER_HPP_

#include <pd/pd.hpp>
#include <pd/resource_base.hpp>
#include <map>
#include <vector>

namespace pd {

    class resource_manager {
    public:
        resource_manager();
        ~resource_manager();

        void push();
        void pop();
        size_t stack_size() { return m_stack.size(); }
        size_t size() { return m_size; }

        template <class T>
        T *get(const std::string &filename)
        {
            std::map<std::string, pd::resource_base *>::iterator iter;
            for (int i = m_stack.size() - 1; i >= 0; i--) {
                iter = m_stack[i].find(filename);
                if (iter != m_stack[i].end()) {
                    T *ptr = dynamic_cast<T *>(iter->second);
                    assert(ptr);
                    return ptr;
                }
            }
            T *rv = T::load_as_resource(filename);
            rv->m_resmgr = this;
            m_stack[m_stack.size() - 1][filename] = rv;
            m_size++;
            return rv;
        }

    private:
        resource_manager(const resource_manager &resource_manager);
        resource_manager &operator=(const resource_manager &resource_manager);

        std::vector<std::map<std::string, pd::resource_base *> > m_stack;
        size_t m_size;
    };
}

#endif
