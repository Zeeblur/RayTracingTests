#pragma once

#include "ray.h"
#include "utils.h"

class camera {
public:
	camera(vec3 lookfrom, vec3 lookat, vec3 vup, float vofx, float aspect, float aperture, float focus_dist); 
	~camera() {};

	ray get_ray(float s, float t);

	vec3 origin;
	vec3 lower_left_corner;
	vec3 horizontal;
	vec3 vertical;
	float lens_radius;
	vec3 u, v, w;

	vec3 random_unit_disk()
	{
		vec3 p;

		do {
			p = 2.0f * vec3(Utils::unif(Utils::rng), Utils::unif(Utils::rng), 0) - vec3(1,1,0);
		} while (dot(p, p) >= 1.0);

		return p;
	}
};