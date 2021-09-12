//
// Created by Robin Leman on 2021-08-21.
// robin.leman@mail.mcgill.ca
// Based of : https://raytracing.github.io/books/RayTracingInOneWeekend.html
//

#ifndef RAYTRACER_HITTABLE_H
#define RAYTRACER_HITTABLE_H

#include "raytracer.h"

class material;

struct hit_info
{
    point3 p;
    vec3 n;
    double t;
    bool front_face;
    shared_ptr<material> mat;

    inline void set_face_normal(const ray& r, const vec3& outward_normal)
    {
        front_face = dot(r.dir, outward_normal) < 0;
        n = front_face ? outward_normal : -outward_normal;
    }
};

class hittable
{
public:
    virtual  bool hit(ray& r, double t_min, double t_max, hit_info& out) const = 0;
};

#endif //RAYTRACER_HITTABLE_H
