#pragma once
#include <glm/glm.hpp>

using namespace glm;

struct ray
{
	ray() {}
	ray(const vec3& a, const vec3& b) : A(a)
	{
		// normalise direction to unit vector
		B = normalize(b);
	}	
	
	vec3 origin() const			  { return A; }
	vec3 direction() const		  { return B; }
	vec3 hit_point(float t) const { return A + t * B; }

	vec3 A;
	vec3 B;
};