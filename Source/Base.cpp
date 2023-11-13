#include "../Header/Base.h"

float lerp(float a, float b, float t)
{
    return a + (t * (b - a));
}
double lerp(double a, double b, double t)
{
    return a + (t * (b - a));
}