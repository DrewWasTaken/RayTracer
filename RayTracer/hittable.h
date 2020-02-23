#pragma once
#ifndef HITTABLEH
#define HITTABLEH

#include "Aabb.h"
#include "Ray.h"
#include <memory>
#include <optional>
#include <utility>

class material;

struct hit_record {
    float t;
    float u;
    float v;
    vec3 p;
    vec3 normal;
    material* mat_ptr;
};

class hittable
{
	public:
		virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const = 0;
		virtual bool bounding_box(float t0, float t1, aabb& box) const = 0;
};

class flip_normals : public hittable //Makes Outer Walls Visible Via Flipping
{
public:
    flip_normals(hittable* p) : ptr(p) {}

    virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const
    {
        if (ptr->hit(r, t_min, t_max, rec))
        {
            rec.normal = -rec.normal;
            return true;
        }
        else
            return false;
    }

    virtual bool bounding_box(float t0, float t1, aabb& box) const
    {
        return ptr->bounding_box(t0, t1, box);
    }

    hittable* ptr;
};
#endif