//
// Created by Robin Leman on 2021-08-21.
//

#ifndef RAYTRACER_CAMERA_H
#define RAYTRACER_CAMERA_H

class camera
{
public:

public:
    camera(point3 lookfrom, point3 lookat, vec3 vup, double vfov, double aspect_ratio, double aperture, double focus_dist)
    {
        double theta = degrees_to_radians(vfov);
        double h = tan(theta/2);
        double viewport_height = 2 * h;
        double viewport_width = aspect_ratio * viewport_height;

        vec3 w = unit_vector(lookfrom-lookat);
        vec3 u = unit_vector(cross(vup, w));
        vec3 v = cross(w, u);

        origin = lookfrom;
        horizontal = viewport_width*u*focus_dist;
        vertical = viewport_height*v*focus_dist;

        lower_left_corner = origin -horizontal/2 -vertical/2 - focus_dist*w;

        length_radius = aperture / 2;
    }

    ray get_ray(double s, double t)
    {
        vec3  rd = length_radius * random_in_unit_sphere();
        vec3 offset = u*rd.x() + v*rd.y();
        return ray(origin + offset, lower_left_corner + s*horizontal + t*vertical - origin - offset);
    }

    vec3 vertical;
    vec3 horizontal;
    point3 origin;
    point3 lower_left_corner;
    vec3 u, v, w;
    double length_radius;

    const int image_width = 1200;
    const int image_height = static_cast<int>(image_width / (3.0/2.0));
    const int samples_per_pixel = 100;
    const int max_depth = 10;
};

#endif //RAYTRACER_CAMERA_H
