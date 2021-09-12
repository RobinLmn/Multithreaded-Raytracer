//
// Created by Robin Leman on 2021-08-21.
//

#ifndef RAYTRACER_SPHERE_H
#define RAYTRACER_SPHERE_H

#include "hittable.h"

class sphere : public hittable
{
public:
    point3 center;
    double radius;
    shared_ptr<material> mat;

    sphere() : center(), radius(0) {}
    sphere(point3 c, double r, shared_ptr<material> m) : center(c), radius(r), mat(m) {}

    virtual bool hit(ray& r, double t_min, double t_max, hit_info& out) const override;
};

bool sphere::hit(ray& r, double t_min, double t_max, hit_info& out) const
{
    vec3 oc = r.origin() - center;
    double a = r.direction().length_squared();
    double half_b = dot(r.direction(), oc);
    double c = oc.length_squared() - radius * radius;
    double discriminant = half_b*half_b - a*c;

    if (discriminant < 0) return false;

    double sqrt_disc = sqrt(discriminant);
    double root = (- half_b - sqrt_disc) / a;

    if (root < t_min || root > t_max)
    {
        root = (- half_b + sqrt_disc) / a;
        if (root < t_min || root > t_max)
        {
            return false;
        }
    }

    out.t = root;
    out.p = r.at(out.t);
    vec3 outward_normal = (out.p - center) / radius;
    out.set_face_normal(r, outward_normal);
    out.mat = mat;

    return true;
};

#endif //RAYTRACER_SPHERE_H
