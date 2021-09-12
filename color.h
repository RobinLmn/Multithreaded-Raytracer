//
// Created by Robin Leman on 2021-08-21.
// robin.leman@mail.mcgill.ca
// Based of : https://raytracing.github.io/books/RayTracingInOneWeekend.html
//

#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"
#include <iostream>

void write_color(std::ostream& out, const color pixel_color, int samples_per_pixel)
{
    double r = pixel_color.x();
    double g = pixel_color.y();
    double b = pixel_color.z();

    const double scale = 1.0 / samples_per_pixel;   // sqrt for gamma correction
    r = sqrt(r * scale);
    b = sqrt(b * scale);
    g = sqrt(g * scale);

    int ir = static_cast<int>(256 * clamp(r, 0, 0.9999));
    int ig = static_cast<int>(256 * clamp(g, 0, 0.9999));
    int ib = static_cast<int>(256 * clamp(b, 0, 0.9999));

    out << ir << ' '<< ig << ' ' << ib << '\n';
}

vec3 blend(const double t, const color startColor, const color endColor)
{
    return (1-t) * startColor + t * endColor;
}

#endif