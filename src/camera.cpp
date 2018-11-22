#include "camera.h"
#include <glm/gtc/constants.hpp>

camera::camera(vec3 lookfrom, vec3 lookat, vec3 vup, float vfov, float aspect, float aperture, float focus_dist) // vertical field of view top to bottom in degrees
{
	lens_radius = aperture / 2;


	float theta = vfov * pi<float>() / 180;
	float half_height = tan(theta / 2);
	float half_width = aspect * half_height;

	origin = lookfrom;
	w = normalize(lookfrom - lookat);
	u = normalize(cross(vup, w));
	v = cross(w, u);

	lower_left_corner = origin - half_width * focus_dist * u - half_height * focus_dist * v - focus_dist * w;

	horizontal = 2 * half_width * u;
	vertical = 2 * half_height * v;
}


ray camera::get_ray(float s, float t)
{
	vec3 rd = lens_radius * random_unit_disk();
	vec3 offset = u * rd.x +  v * rd.y;

	return ray(origin + offset, lower_left_corner + s * horizontal + t * vertical - origin - offset); 
}

