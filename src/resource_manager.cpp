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
    m_stack.push_back(std::map<std::string, pd::resource_base *>());
}

void pd::resource_manager::pop()
{
    std::map<std::string, pd::resource_base *> &loaded = m_stack[m_stack.size() - 1];
    for (std::map<std::string, pd::resource_base *>::iterator iter = loaded.begin();
         iter != loaded.end(); ++iter)
        delete iter->second;
    m_size -= loaded.size();
    m_stack.pop_back();
}
