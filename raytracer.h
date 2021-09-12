//
// Created by Robin Leman on 2021-08-21.
//

#ifndef RAYTRACER_RAYTRACER_H
#define RAYTRACER_RAYTRACER_H

#define PI 3.1415926535897932385

#include <cmath>
#include <limits>
#include <memory>
#include <random>

using std::shared_ptr;
using std::make_shared;
using std::sqrt;

const double infinity = std::numeric_limits<double>::infinity();

inline double degrees_to_radians(double d)
{
    return d * PI / 180.0;
}

inline double random_double() {
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    static std::mt19937 generator;
    return distribution(generator);
}

inline double random_double(double min, double max)
{
    return random_double() * (max-min) + min;
}

inline double clamp(double x, double min, double max)
{
    if (x < min) return min;
    if (x > max) return max;
    return x;
}

#include "ray.h"
#include "vec3.h"

#endif //RAYTRACER_RAYTRACER_H
