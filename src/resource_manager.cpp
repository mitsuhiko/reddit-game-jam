#include <pd/resource_manager.hpp>


pd::resource_manager::resource_manager()
{
    m_size = 0;
    push();
}

pd::resource_manager::~resource_manager()
{
    pop();
}

void pd::resource_manager::push()
{
    m_stack.push_back(map_t());
}

void pd::resource_manager::pop()
{
    map_t &loaded = m_stack[m_stack.size() - 1];
    for (map_t::iterator iter = loaded.begin(); iter != loaded.end(); ++iter)
        delete iter->second;
    m_size -= loaded.size();
    m_stack.pop_back();
}
