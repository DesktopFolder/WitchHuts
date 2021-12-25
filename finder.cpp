#include <fstream>
#include <iostream>
#include <optional>

#include "cubiomes.hpp"
#include "parser.hpp"

constexpr int witchHutCount = 2;

struct OutputManager
{
    std::optional<std::string> filename;

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

private:
    std::ofstream file;

    template <typename T>
    void _write(const T& s)
    {
        if (filename)
        {
            if (not file.is_open())
            {
                file.open(*filename);
            }
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
    while (not ap.done())
    {
        // Parse arguments real quick.
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
                output.filename = arg.name;
            }
        }
    }

    long searchRange = 500;
    int offset       = witchHutCount - 4;
    Point qhpos[4];
    int results[3] = {0, 0, 0};

    SeedContext context{};

    seed_t seed = SEED_USED;
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
                        output.write("CENTER for ", maxi, " huts: ", x, ", ", z);
                        // fprintf(fp, "CENTER for %d huts: %d,%d\n", maxi, x, z);
                        results[maxi - 2]++;
                    }
                }
            }
        }
    }

    return 0;
}
