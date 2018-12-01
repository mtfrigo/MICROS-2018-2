#include <util.h>

float util_map(int value, int from_min, int from_max, int to_min, int to_max)
{
    int from_range = from_max - from_min;
    int to_range = to_max - to_min;

    float proportion = ((float)from_range)/to_range;

    return value / proportion;
}