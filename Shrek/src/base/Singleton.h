#pragma once
#include "defs.h"
#include <string_view>
#include "platform/Log.h"


namespace shrek::base {

template <typename Type>
class Singleton
{
public:
    Singleton(std::string_view name) SRK_NOEXCEPT :
        m_Name(name)
    {
        if (s_IsOnly)
        {
            SRK_CORE_ERROR("{} has already been created once!!", m_Name);
            std::exit(-1);
        }
        s_IsOnly = true;
    }

private:
    static bool s_IsOnly;

private:
    std::string_view m_Name;
};


template <typename Type>
bool Singleton<Type>::s_IsOnly = false;

} // namespace shrek::interface

