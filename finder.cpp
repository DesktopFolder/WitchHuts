#include <iostream>
#include <cstdint>
#include "finders.h"
#include "layers.h"
#include "generator.h"

const int witchHutCount = 2;

int euclideanDistance(int x1, int y1, int x2, int y2)
{
    double dx = (x1 - x2);
    double dy = (y1 - y2);
    return (int)(dx * dx + dy * dy);
}

Pos getStructurePos(StructureConfig sconf, uint64_t seed, int rx, int rz)
{
    Pos p;
    getStructurePos(sconf.structType, MC_1_18, seed, rx, rz, &p);
    return p;
}

int getBiomeAtPos(Generator *g, Pos *pos)
{
    return getBiomeAt(g, 1, pos->x, 64, pos->z);
}

int main()
{
    long searchRange = 500;
    int offset = witchHutCount - 4;
    Pos qhpos[4];
    int results[3] = {0, 0, 0};

    Generator g;
    setupGenerator(&g, MC_1_18, 0);

    auto featureConfig = SWAMP_HUT_CONFIG;
    uint64_t seed = SEED_USED;
    for (int regPosX = -searchRange; regPosX < searchRange; ++regPosX)
    {
        for (int regPosZ = -searchRange; regPosZ < searchRange; ++regPosZ)
        {
            int skipTest = 0;
            qhpos[0] = getStructurePos(featureConfig, seed, 0 + regPosX, 0 + regPosZ);
            qhpos[1] = getStructurePos(featureConfig, seed, 0 + regPosX, 1 + regPosZ);
            if (euclideanDistance(qhpos[0].x, qhpos[0].z, qhpos[1].x, qhpos[1].z) < 65536)
            {
                skipTest = 1;
            }
            qhpos[2] = getStructurePos(featureConfig, seed, 1 + regPosX, 0 + regPosZ);
            if (skipTest ||
                euclideanDistance(qhpos[0].x, qhpos[0].z, qhpos[2].x, qhpos[2].z) < 65536 ||
                euclideanDistance(qhpos[1].x, qhpos[1].z, qhpos[2].x, qhpos[2].z) < 65536)
            {
                skipTest = 1;
            }
            qhpos[3] = getStructurePos(featureConfig, seed, 1 + regPosX, 1 + regPosZ);
            if (skipTest ||
                euclideanDistance(qhpos[0].x, qhpos[0].z, qhpos[3].x, qhpos[3].z) < 65536 ||
                euclideanDistance(qhpos[1].x, qhpos[1].z, qhpos[3].x, qhpos[3].z) < 65536 ||
                euclideanDistance(qhpos[2].x, qhpos[2].z, qhpos[3].x, qhpos[3].z) < 65536)
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
            applySeed(&g, 0, seed);
            int correctPos[4] = {-1, -1, -1, -1};
            if (getBiomeAtPos(&g, &qhpos[0]) == swampland)
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
            if (getBiomeAtPos(&g, &qhpos[1]) == swampland)
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
            if (getBiomeAtPos(&g, &qhpos[2]) == swampland)
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
            if (getBiomeAtPos(&g, &qhpos[3]) == swampland)
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
                        if (euclideanDistance(
                                qhpos[correctPos[i]].x, qhpos[correctPos[i]].z, x, z) > 16384)
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
