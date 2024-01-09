#include "mysphere.h"

int32_t get_cap_area( double r, double a, double b, double c, double d, double *pArea )
{
    if (r <= 0) return -1;
    if (a == 0 && b == 0 && c == 0) return -1;

    double distance = fabs(d) / sqrt((a * a) + (b * b) + (c * c));
    if (distance > r) return 0;

    double cap_radius = sqrt( pow(r, 2) - pow(distance, 2));

    *pArea = M_PI * pow(cap_radius, 2);

    if (*pArea == 0.0) return 0;

    return 1;
}