/**
 * C++ wrapper for Cubiomes
 * Header-only; minimal wrapper
 */

// Cubiomes includes
#include "finders.h"
#include "generator.h"
#include "layers.h"

// Other includes
#include <cstdint>

/* Helper values */
struct MCConfig
{
    static constexpr auto version = MC_1_18;
};

/* Helper types */
struct Point
{
    int x{};
    int z{};
};

[[nodiscard]] constexpr int distance_squared(Point a, Point b)
{
    Point d{a.x - b.x, a.z - b.z};
    return (d.x * d.x + d.z * d.z);
}

using seed_t = uint64_t;

// Quick wrapper for cubiomes' generators
// This way we don't need to call setup functions. Nice!
struct SeedContext
{
    Generator generator;

    SeedContext()
    {
        // I love C APIs
        setupGenerator(&generator, MCConfig::version, 0 /* No flags */);
    }

    void applySeed(seed_t seed)
    {
        // I just do. They're so nice.
        // So many magic values. So many """enums""".
        // It's great when every function call requires a comment!
        ::applySeed(&generator, 0 /* Overworld */, seed);
    }

    int getBiomeAt(Point p)
    {
        // We hardcode the Y level we search at for witch huts
        // Alternatively, we could take this as a template param
        constexpr int yLevel = 64;
        return ::getBiomeAt(&generator, 1 /* Get biome at block position (cool flag system :D)*/,
                          p.x, 64, p.z);
    }
};

// Cubiomes -> getStructurePos wrapper.
[[nodiscard]] Point getWitchHutPosition(seed_t seed, Point pt)
{
    Pos p;
    // ewwwwwwwwwwwwwwww
    // it's 2021, we don't need to pass references to a struct
    // with 2 ints to avoid extra stack allocations...........
    getStructurePos(Swamp_Hut, MCConfig::version, seed, pt.x, pt.z, &p);
    return {p.x, p.z};
}

int getBiomeAt(const SeedContext& s, Point p)
{
    // We hardcode the Y level we search at for witch huts
    // Alternatively, we could take this as a template param
    constexpr int yLevel = 64;
    return getBiomeAt(&s.generator, 1 /* Get biome at block position (cool flag system :D)*/, p.x,
                      64, p.z);
}
