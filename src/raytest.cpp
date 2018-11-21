#include <iostream>
#include <random>
#include <fstream>
#include <glm/glm.hpp>

#include "ray.h"
#include "sphere.h"
#include "hitable_list.h"
#include "camera.h"

using namespace std;
using namespace glm; 


// linearly blends white and blue depending on the up/downess of the y coordinate
// essentially lerp between blue and white with clamped values: blended_value = (1-t)*start_value + t*end_value
vec3 colour(const ray& r, hitable *world)
{
	hit_record rec;

	if (world->hit(r, 0.0, _MM_FIXUP_MAX_FLOAT, rec))
	{
		return 0.5f*vec3(rec.normal.x + 1, rec.normal.y + 1, rec.normal.z + 1);
	}
	else
	{
		float t = 0.5f*(r.direction().y + 1.0f);
		return (1.0f - t) * vec3(1.0) + t * vec3(0.5, 0.7, 1.0);
	}
}

int main()
{
	int nx = 200;
	int ny = 100;
	int ns = 100;
	ofstream ss("output.ppm");
	if (!ss.is_open())
		return 1;


	ss << "P3\n" << nx << " " << ny << " \n255\n";

	hitable *list[2];
	list[0] = new sphere(vec3(0, 0, -1), 0.5);
	list[1] = new sphere(vec3(0, -100.5, -1), 100);
	hitable *world = new hitable_list(list, 2);

	camera* cam = new camera();

	for (int j = ny - 1; j >= 0; j--)
	{
		for (int i = 0; i < nx; i++)
		{
			vec3 col(0);

			for (int s = 0; s < ns; s++)
			{
				// calculate uv coords
				float u = float(i + (rand()/RAND_MAX) + 1) / float(nx);
				float v = float(j + (rand()/RAND_MAX) + 1) / float(ny);

				ray r = cam->get_ray(u, v);
				vec3 p = r.hit_point(2.0);
				col += colour(r, world);
			}

			col /= float(ns);
			int ir = int(255.99 * col[0]);
			int ig = int(255.99 * col[1]);
			int ib = int(255.99 * col[2]);

			ss << ir << " " << ig << " " << ib << "\n";
		}
	}

	ss.close();

	return 0;
}