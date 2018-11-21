#include <iostream>
#include <random>
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>
#include <chrono>

#include "ray.h"
#include "sphere.h"
#include "hitable_list.h"
#include "camera.h"

using namespace std;
using namespace glm; 


std::mt19937_64 rng;
std::uniform_real_distribution<double> unif(0, 1);

vec3 random_in_unit_sphere()
{
	vec3 p;
	do {
		p = 2.0f * vec3(unif(rng), unif(rng), unif(rng)) - vec3(1.0);
	} while (length2(p) >= 1.0);

	return p;
}


// linearly blends white and blue depending on the up/downess of the y coordinate
// essentially lerp between blue and white with clamped values: blended_value = (1-t)*start_value + t*end_value
vec3 colour(const ray& r, hitable *world)
{
	hit_record rec;

	// 2nd parameter is epsilon to reduce rounding errors.
	if (world->hit(r, 0.001, _MM_FIXUP_MAX_FLOAT, rec))
	{
		vec3 target = rec.p + rec.normal + random_in_unit_sphere();

		return 0.5f*colour(ray(rec.p, target - rec.p), world);
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


	// random init

	// initialize the random number generator with time-dependent seed
	uint64_t timeSeed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
	std::seed_seq seed{ uint32_t(timeSeed & 0xffffffff), uint32_t(timeSeed >> 32) };
	rng.seed(seed);
	// initialize a uniform distribution between 0 and 1
	//unif(0, 1);


	for (int j = ny - 1; j >= 0; j--)
	{
		for (int i = 0; i < nx; i++)
		{
			vec3 col(0);

			for (int s = 0; s < ns; s++)
			{
				// calculate uv coords
				float u = float(i + unif(rng)) / float(nx);
				float v = float(j + unif(rng)) / float(ny);

				ray r = cam->get_ray(u, v);
				vec3 p = r.hit_point(2.0);
				col += colour(r, world);
			}

			col /= float(ns);
			col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
			int ir = int(255.99 * col[0]);
			int ig = int(255.99 * col[1]);
			int ib = int(255.99 * col[2]);

			ss << ir << " " << ig << " " << ib << "\n";
		}
	}

	ss.close();

	return 0;
}