//
// Created by Robin Leman on 2021-08-21.
// robin.leman@mail.mcgill.ca
// Based of : https://raytracing.github.io/books/RayTracingInOneWeekend.html
//

#ifndef RAYTRACER_RAY_H
#define RAYTRACER_RAY_H

#include "vec3.h"

class ray
{
public:

    point3 orig;
    vec3 dir;

    ray() : orig(), dir() {}
    ray(point3 origin, vec3 direction) : orig(origin), dir(direction) {}

    vec3& direction() { return dir; }
    point3& origin() { return orig; }

    point3 at(const double t) { return dir * t + orig; }
};

#endif //RAYTRACER_RAY_H
