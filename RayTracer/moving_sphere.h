#pragma once
#include "hitable.h"

class moving_sphere : public hitable
{
	public:
		moving_sphere() {}
		moving_sphere(vec3 cen0, vec3 cen1, float t0, float t1, float r, material* m) :
			center0(cen0), center1(cen1), time0(t0), time1(t1), radius(r), mat_ptr(m) {};
		virtual bool hit(const ray& r, float tmin, float tmax, hit_record& rec) const;
		vec3 center(float time) const;
		vec3 center0, center1;
		float time0, time1;
		float radius;
		material *mat_ptr;
};

vec3 moving_sphere::center(float time) const
{
	return center0 + ((time - time0) / (time1 - time0)) * (center1 - center0);
}

aabb surrounding_box(aabb box0, aabb box1)
{
	vec3 small(fmin(box0.min().x(), box1.min().x()),
		fmin(box0.min().y(), box1.min().y()),
		fmin(box0.min().z(), box1.min().z()));

	vec3 big(fmax(box0.min().x(), box1.max().x()),
		fmax(box0.min().y(), box1.max().y()),
		fmax(box0.min().z(), box1.max().z()));
	return aabb(small, big);
}