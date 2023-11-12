#include "../Header/Base.h"

internal float lerp(float a, float b, float t)
{
    return a + (t * (b - a));
}
internal double lerp(double a, double b, double t)
{
    return a + (t * (b - a));
}