#pragma once
#include "defs.h"
#include <cstdint>
#include <optional>

namespace shrek::render::helper {

struct QueueFamilyIndices
{
    uint32_t Graphics;
    std::optional<uint32_t> Compute;
};

}
