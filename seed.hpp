#pragma once

#include <optional>

constexpr bool optimize_seed()
{
#ifdef OPTIMIZE_SEED
    return true;
#endif
    return false;
}
