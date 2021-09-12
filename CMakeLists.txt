cmake_minimum_required(VERSION 3.17)
project(RayTracer)

set(CMAKE_CXX_STANDARD 14)

add_executable(RayTracer main.cpp vec3.h color.h ray.h hittable.h sphere.h hittable_list.h raytracer.h camera.h material.h)