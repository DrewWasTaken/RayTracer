#pragma once
#ifndef RAYH
#define RAYH
#include "vec3.h"

class ray
{
public:
	ray() {}
	ray(const vec3& a, const vec3& b, float ti = 0.0) { A = a; B = b; _time = ti;} // P(t) = A + t*B

	vec3 origin() const				{return A;} // A = Ray Origin
	vec3 direction() const			{return B;} // B = Ray Direction
	float time() const				{return _time;}
	vec3 position(float t) const	{return A + t * B;} // P = 3D Position

	vec3 A;
	vec3 B;
	float _time;
};

#endif