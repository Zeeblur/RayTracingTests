#include <iostream>
#include <fstream>
#include <glm/glm.hpp>

#include "ray.h"

using namespace std;
using namespace glm; 

float hit_sphere(const vec3& center, float radius, const ray& r)
{
	vec3 oc = r.origin() - center; 
	float a = dot(r.direction(), r.direction());
	float b = 2.0 * dot(oc, r.direction());
	float c = dot(oc, oc) - radius * radius;
	float discriminant = b * b - 4 * a*c;

	if (discriminant < 0)
	{
		return -1.0; // not a hit
	}
	else
	{
		// quadratic eq solved for t
		return (-b - sqrt(discriminant)) / 2.0*a;
	}
	
}

// linearly blends white and blue depending on the up/downess of the y coordinate
// essentially lerp between blue and white with clamped values: blended_value = (1-t)*start_value + t*end_value
vec3 colour(const ray& r)
{
	float t = hit_sphere(vec3(0, 0, -1), 0.5, r);
	if (t > 0)
	{
		vec3 N = normalize(r.hit_point(t) - vec3(0, 0, -1));
		return 0.5f*vec3(N.x + 1, N.y + 1, N.z + 1);
	}

	t = 0.5*(r.direction().y + 1.0);
	return (1.0f - t) * vec3(1.0) + t * vec3(0.5, 0.7, 1.0);
}

int main()
{
	int nx = 200;
	int ny = 100;

	ofstream ss("output.ppm");
	if (!ss.is_open())
		return 1;


	ss << "P3\n" << nx << " " << ny << " \n255\n";

	// set up view frustrum bounds
	vec3 lower_left_corner(-2.0, -1.0, -1.0);
	vec3 horizontal(4.0, 0.0, 0.0);
	vec3 vertical(0.0, 2.0, 0.0);
	vec3 origin(0.0);
	
	for (int j = ny - 1; j >= 0; j--)
	{
		for (int i = 0; i < nx; i++)
		{
			// calculate uv coords
			float u = float(i) / float(nx);
			float v = float(j) / float(ny);

			ray r(origin, lower_left_corner + u * horizontal + v * vertical);

			vec3 col = colour(r);
			int ir = int(255.99 * col[0]);
			int ig = int(255.99 * col[1]);
			int ib = int(255.99 * col[2]);

			ss << ir << " " << ig << " " << ib << "\n";
		}
	}

	ss.close();

	return 0;
}