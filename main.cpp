//
// Created by Robin Leman on 2021-08-21.
// robin.leman@mail.mcgill.ca
// Based of : https://raytracing.github.io/books/RayTracingInOneWeekend.html
//

#include <iostream>
#include <thread>

#include "raytracer.h"
#include "color.h"
#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"
#include "material.h"


color ray_color(ray& r, hittable& world, int depth)
{
    if (depth <= 0) return {};

    hit_info out;
    if (world.hit(r, 0.001, infinity, out))
    {
        ray scattered_ray;
        color attenuation;
        if (out.mat->scatter(r, out, attenuation, scattered_ray))
            return attenuation * ray_color(scattered_ray, world, depth-1);
        return {};
    }

    vec3 unit_direction = unit_vector(r.direction());
    double t = 0.5 * (unit_direction.y() + 1);  // brings y from [-1;1] to [0;1]
    return blend(t, color(1, 1, 1), color(0.5, 0.7, 1));
}

hittable_list random_scene() {
    hittable_list world;

    auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    world.add(make_shared<sphere>(point3(0,-1000,0), 1000, ground_material));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random_double();
            point3 center(a + 0.9*random_double(), 0.2, b + 0.9*random_double());

            if ((center - point3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = color::random() * color::random();
                    sphere_material = make_shared<lambertian>(albedo);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = color::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<metal>(albedo, fuzz);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else {
                    // glass
                    sphere_material = make_shared<dielectric>(1.5);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = make_shared<dielectric>(1.5);
    world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
    world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

    return world;
}

void draw(camera cam, hittable_list world, int start, int end, std::vector<color>* out) {
    int i = (cam.image_height - start) * cam.image_width;

    for (int y = start - 1; y >= end; y--)
    {

        for (int x = 0; x < cam.image_width; x++)
        {
            color pixel_color;
            for (int s = 0; s < cam.samples_per_pixel; s++) {
                double u = (x + random_double()) / (cam.image_width - 1);
                double v = (y + random_double()) / (cam.image_height - 1);
                ray r = cam.get_ray(u, v);
                pixel_color += ray_color(r, world, cam.max_depth);
            }
            out->at(i) = pixel_color;
            i++;
        }
    }
}

int main() {

    // IMAGE

    const auto aspect_ratio = 3.0 / 2.0;

    point3 lookfrom(13,2,3);
    point3 lookat(0,0,0);
    vec3 vup(0,1,0);
    auto dist_to_focus = 10.0;
    auto aperture = 0.1;

    camera camera(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus);

    std::cout << "P3\n" << camera.image_width << ' ' << camera.image_height << "\n255\n";

    hittable_list world = random_scene();
    std::vector<color> image(camera.image_height * camera.image_width);

    // Init threads

    int num_threads = 16;
    std::vector<std::thread> threads(num_threads);

    int segment = camera.image_height / num_threads;
    int segment_start = camera.image_height;
    int segment_end = segment_start - segment;

    for (int i=0; i < num_threads; i++)
    {
        threads[i] = std::thread(draw, camera, world, segment_start, segment_end, &image);
        segment_start = segment_end;
        segment_end -= segment;
    }

    for (int i=0; i < num_threads; i++)
    {
        threads[i].join();
    }

    std:: cerr << "\nWriting...\n";

    for (color c : image)
    {
        write_color(std::cout, c, camera.samples_per_pixel);
    }

    std::cerr << "\nDone.\n";
}
