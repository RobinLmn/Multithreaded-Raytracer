//
// Created by Robin Leman on 2021-08-22.
//

#ifndef RAYTRACER_MATERIAL_H
#define RAYTRACER_MATERIAL_H

#include "raytracer.h"

struct hit_info;

class material
{
public:
    virtual bool scatter(const ray& ray_in, const hit_info& out, color& attenuation, ray& ray_out) const = 0;
};


class lambertian : public material
{
public:
    lambertian(const color& a) : albedo(a) {}

    bool scatter(const ray& ray_in, const hit_info& out, color& attenuation, ray& ray_out) const override
    {
        attenuation = albedo;
        vec3 scatter_dir = out.n + unit_vector(random_in_unit_sphere());
        if (scatter_dir.near_zero()) scatter_dir = out.n;
        ray_out = ray(out.p, scatter_dir);
        return true;
    }

    color albedo;
};

class metal : public material
{
public:
    metal(const color& a, const double f) : albedo(a), fuzz(f < 1 ? f : 1) {}

    bool scatter(const ray& ray_in, const hit_info& out, color& attenuation, ray& ray_out) const override
    {
        attenuation = albedo;
        vec3 reflected_dir = reflect(unit_vector(ray_in.dir), out.n) + fuzz * unit_vector(random_in_unit_sphere());
        ray_out = ray(out.p, reflected_dir);
        return dot(reflected_dir, out.n) > 0;
    }

    color albedo;
    double fuzz;
};

class dielectric : public material
{
public:
    dielectric(const double index_of_refraction) : ir(index_of_refraction) {}

    bool scatter(const ray& ray_in, const hit_info& out, color& attenuation, ray& ray_out) const override
    {
        attenuation = color(1.0, 1.0, 1.0);
        double refraction_ratio = out.front_face ? 1.0 / ir : ir;

        vec3 unit_direction(unit_vector(ray_in.dir));
        vec3 direction;

        double cos_theta = fmin(dot(-unit_direction, out.n), 1.0);
        double sin_theta = sqrt(1.0 - cos_theta*cos_theta);

        if (refraction_ratio * sin_theta > 1.0 || reflectance(cos_theta, refraction_ratio) > random_double())
        {
            direction = reflect(unit_direction, out.n);
        }
        else {
            direction = refract(unit_direction, out.n, refraction_ratio);
        }

        ray_out = ray(out.p, direction);
        return true;
    }
    double ir;

private:
    static double reflectance(double cosine, double ref_idx) {
        // Use Schlick's approximation for reflectance.
        auto r0 = (1-ref_idx) / (1+ref_idx);
        r0 *= r0;
        return r0 + (1-r0)*pow((1 - cosine),5);
    }
};
#endif //RAYTRACER_MATERIAL_H
