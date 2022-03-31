#pragma once

// forward declare
namespace shrek {
namespace settings {

#ifndef SRK_NO_DEBUG
constexpr bool ShouldDebug = false;

#else
constexpr bool ShouldDebug = true;
#endif
} // namespace settings


} // namespace shrek

#include <cassert>

#ifndef SRK_DIST
#    define SRK_ASSERT(b, msg) assert(b && "msg")
#else
#    define SRK_ASSERT(b, msg)
#endif



#define SRK_NOEXCEPT noexcept
