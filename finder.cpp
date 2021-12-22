#include <iostream>

#include "cubiomes.hpp"

constexpr int witchHutCount = 2;

int main()
{
    long searchRange = 500;
    int offset = witchHutCount - 4;
    Point qhpos[4];
    int results[3] = {0, 0, 0};

    SeedContext context{};

    seed_t seed = SEED_USED;
    for (int regPosX = -searchRange; regPosX < searchRange; ++regPosX)
    {
        for (int regPosZ = -searchRange; regPosZ < searchRange; ++regPosZ)
        {
            int skipTest = 0;
            qhpos[0] = getWitchHutPosition(seed, {0 + regPosX, 0 + regPosZ});
            qhpos[1] = getWitchHutPosition(seed, {0 + regPosX, 1 + regPosZ});
            if (distance_squared(qhpos[0], qhpos[1]) < 65536)
            {
                skipTest = 1;
            }
            qhpos[2] = getWitchHutPosition(seed, {1 + regPosX, 0 + regPosZ});
            if (skipTest ||
                distance_squared(qhpos[0], qhpos[2]) < 65536 ||
                distance_squared(qhpos[1], qhpos[2]) < 65536)
            {
                skipTest = 1;
            }
            qhpos[3] = getWitchHutPosition(seed, {1 + regPosX, 1 + regPosZ});
            if (skipTest ||
                distance_squared(qhpos[0], qhpos[3]) < 65536 ||
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
            if (getBiomeAt(context, qhpos[0]) == swampland)
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
            if (getBiomeAt(context, qhpos[1]) == swampland)
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
            if (getBiomeAt(context, qhpos[2]) == swampland)
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
            if (getBiomeAt(context, qhpos[3]) == swampland)
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
                    int x = 0;
                    int z = 0;
                    for (int i = 0; i < maxi; ++i)
                    {
                        x += qhpos[correctPos[i]].x;
                        z += qhpos[correctPos[i]].z;
                    }
                    x = (int)(x / (double)maxi);
                    z = (int)(z / (double)maxi);
                    int valid = 1;
                    for (int i = 0; i < maxi; ++i)
                    {
                        if (distance_squared(
                                qhpos[correctPos[i]], {x, z}) > 16384)
                            valid = 0;
                    }
                    if (valid && maxi >= offset + 4)
                    {
                        printf("CENTER for %d huts: %d,%d\n", maxi, x, z);
                        // fprintf(fp, "CENTER for %d huts: %d,%d\n", maxi, x, z);
                        results[maxi - 2]++;
                    }
                }
            }
        }
    }

    return 0;
}
