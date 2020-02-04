#pragma once
#ifndef RAYH
#define RAYH
#include "vec3.h"

class ray
{
public:
	ray() {}
	ray(const vec3& a, const vec3& b) {A = a; B = b;} // P(t) = A + t*B
	vec3 origin() const {return A;} // A = Ray Origin
	vec3 direction() const {return B;} // B = Ray Direction
	vec3 position(float t) const {return A + t * B;} // P = 3D Position

	vec3 A;
	vec3 B;
};

#endif