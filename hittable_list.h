//
// Created by Robin Leman on 2021-08-21.
//

#ifndef RAYTRACER_HITTABLE_LIST_H
#define RAYTRACER_HITTABLE_LIST_H

#include <vector>
#include <memory>
#include "hittable.h"

using std::shared_ptr;
using std::make_shared;

class hittable_list : public hittable
{
public:
    std::vector<shared_ptr<hittable>> objects;

    hittable_list() = default;
    hittable_list(shared_ptr<hittable> object) { add(object); }

    void clear() { objects.clear(); }
    void add(shared_ptr<hittable> object) { objects.push_back(object); }

    virtual bool hit(ray& r, double t_min, double t_max, hit_info& out) const;
};

bool hittable_list::hit(ray& r, double t_min, double t_max, hit_info& out) const
{
    hit_info out_temp;
    bool hit_anything = false;
    double closest = t_max;

    for (const shared_ptr<hittable>& object : objects)
    {
        if (object->hit(r, t_min, closest, out_temp))
        {
            hit_anything = true;
            closest = out_temp.t;
            out = out_temp;
        }
    }

    return hit_anything;
}

#endif //RAYTRACER_HITTABLE_LIST_H
