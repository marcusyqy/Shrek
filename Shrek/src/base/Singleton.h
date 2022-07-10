#pragma once
#include "defs.h"
#include <string_view>
#include "platform/Log.h"


namespace shrek::base::details {

template <typename Type, bool>
class Singleton;

template <typename Type>
class Singleton<Type, true>
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

    // force cannot copy
    Singleton(const Singleton& other) = delete;
    Singleton& operator=(const Singleton& other) = delete;

    // move can probably be done but... should we?
    Singleton(Singleton&& other) = delete;
    Singleton& operator=(Singleton&& other) = delete;

private:
    static bool s_IsOnly;

private:
    std::string_view m_Name;
};


template <typename Type>
class Singleton<Type, false>
{
public:
    Singleton([[maybe_unused]] std::string_view name) SRK_NOEXCEPT
    {
    }

    // force cannot copy
    Singleton(const Singleton& other) = delete;
    Singleton& operator=(const Singleton& other) = delete;

    // move can probably be done but... should we?
    Singleton(Singleton&& other) = delete;
    Singleton& operator=(Singleton&& other) = delete;
};

template <typename Type>
bool Singleton<Type, true>::s_IsOnly = false;

} // namespace shrek::base::details

namespace shrek::base {

template <typename Type>
using Singleton = details::Singleton<Type, ::shrek::settings::ShouldDebug>;

}
