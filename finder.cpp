#include <fstream>
#include <iostream>
#include <optional>
#include <string>

#include "cubiomes.hpp"
#include "parser.hpp"
#include "seed.hpp"

constexpr int witchHutCount = 2;

struct OutputManager
{
    std::optional<std::string> filename;
    static constexpr size_t report_steps = 10;

    template <typename S>
    void write(S str)
    {
        _write(str);
        _write('\n');
    }

    template <typename S, typename... Ss>
    void write(S str, Ss... strs)
    {
        _write(str);
        write(std::forward<Ss>(strs)...);
    }

    template <typename... Ss>
    void step(Ss... strs)
    {
        write(std::forward<Ss>(strs)...);
        if (++steps % report_steps == 0)
        {
            std::cout << "Found " << steps << " witch huts...\n";
        }
    }

private:
    std::ofstream file;
    size_t steps{};

    template <typename T>
    void _write(const T& s)
    {
        if (filename)
        {
            if (not file.is_open())
            {
                file.open(*filename);
            }
            file << s;
        }
        else
        {
            std::cout << s;
        }
    }
};

int main(int argc, char* argv[])
{
    ArgumentParser ap(argc, argv);
    // Scoped stuff
    auto output = OutputManager{};
    std::string seed_str;
    while (not ap.done())
    {
        // Parse arguments real quick.
        // Wow, this is hilariously ugly. Oh well.
        // Using libraries is for the weak.
        const auto arg = ap.consume();
        if (arg.name_clean() == "help")
        {
            // why is std format still not really implemented
            std::cout << "Nice try. Read the README.\n";
        }
        else if (arg.name_clean() == "o" || arg.name_clean() == "output")
        {
            // file
            if (arg.values.empty())
            {
                std::cout << "--output/-o requires one argument (a filename)\n";
            }
            else
            {
                output.filename = arg.values[0];
            }
        }
        else if (arg.name_clean() == "s" || arg.name_clean() == "seed")
        {
            if constexpr (optimize_seed())
            {
                std::cout << "--seed/-s ignored (conflicts with compilation parameter)\n";
            }
            else
            {
                if (arg.values.empty())
                {
                    std::cout << "--seed/-s requires one argument (the seed)\n";
                }
                else
                {
                    seed_str = arg.values[0];
                }
            }
        }
    }

    // Annnnd we end up needing macros anyways.
    // Oh well.
#ifdef OPTIMIZE_SEED
    constexpr seed_t seed = OPTIMIZE_SEED;
#else
    if (seed_str == "")
    {
        std::cout << "Error: Seed not provided. Use [--seed SEED] to set.\n";
        return 1;
    }
    const seed_t seed = std::stol(seed_str);
#endif

    long searchRange = 500;
    int offset       = witchHutCount - 4;
    Point qhpos[4];
    int results[3] = {0, 0, 0};

    SeedContext context{};

    for (int regPosX = -searchRange; regPosX < searchRange; ++regPosX)
    {
        for (int regPosZ = -searchRange; regPosZ < searchRange; ++regPosZ)
        {
            int skipTest = 0;
            qhpos[0]     = getWitchHutPosition(seed, {0 + regPosX, 0 + regPosZ});
            qhpos[1]     = getWitchHutPosition(seed, {0 + regPosX, 1 + regPosZ});
            if (distance_squared(qhpos[0], qhpos[1]) < 65536)
            {
                skipTest = 1;
            }
            qhpos[2] = getWitchHutPosition(seed, {1 + regPosX, 0 + regPosZ});
            if (skipTest || distance_squared(qhpos[0], qhpos[2]) < 65536 ||
                distance_squared(qhpos[1], qhpos[2]) < 65536)
            {
                skipTest = 1;
            }
            qhpos[3] = getWitchHutPosition(seed, {1 + regPosX, 1 + regPosZ});
            if (skipTest || distance_squared(qhpos[0], qhpos[3]) < 65536 ||
                distance_squared(qhpos[1], qhpos[3]) < 65536 ||
                distance_squared(qhpos[2], qhpos[3]) < 65536)
            {
                skipTest = 1;
            }
            if (!skipTest)
            {
                continue;
            }
            int areaX = (int)((unsigned int)regPosX << 1u) + 1;
            int areaZ = (int)((unsigned int)regPosZ << 1u) + 1;

            int count = 0;
            context.applySeed(seed);
            int correctPos[4] = {-1, -1, -1, -1};
            if (context.getBiomeAt(qhpos[0]) == swampland)
            {
                correctPos[count++] = 0;
            }
            else
            {
                if (count <= 0 + offset)
                {
                    continue;
                }
            }
            if (context.getBiomeAt(qhpos[1]) == swampland)
            {
                correctPos[count++] = 1;
            }
            else
            {
                if (count <= 1 + offset)
                {
                    continue;
                }
            }
            if (context.getBiomeAt(qhpos[2]) == swampland)
            {
                correctPos[count++] = 2;
            }
            else
            {
                if (count <= 2 + offset)
                {
                    continue;
                }
            }
            if (context.getBiomeAt(qhpos[3]) == swampland)
            {
                correctPos[count++] = 3;
            }
            else
            {
                if (count <= 3 + offset)
                {
                    continue;
                }
            }

            if (count >= 4 + offset)
            {
                for (int j = 0; j < count - 1; ++j)
                {
                    int maxi = count - j;
                    int x    = 0;
                    int z    = 0;
                    for (int i = 0; i < maxi; ++i)
                    {
                        x += qhpos[correctPos[i]].x;
                        z += qhpos[correctPos[i]].z;
                    }
                    x         = (int)(x / (double)maxi);
                    z         = (int)(z / (double)maxi);
                    int valid = 1;
                    for (int i = 0; i < maxi; ++i)
                    {
                        if (distance_squared(qhpos[correctPos[i]], {x, z}) > 16384) valid = 0;
                    }
                    if (valid && maxi >= offset + 4)
                    {
                        output.step("CENTER for ", maxi, " huts: ", x, ", ", z);
                        // fprintf(fp, "CENTER for %d huts: %d,%d\n", maxi, x, z);
                        results[maxi - 2]++;
                    }
                }
            }
        }
    }

    return 0;
}
