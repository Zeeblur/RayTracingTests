#include <iostream>

#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>

#include "utils.h"
#include "ray.h"
#include "sphere.h"
#include "hitable.h"
#include "hitable_list.h"
#include "camera.h"
#include "material.h"

using namespace std;
using namespace glm;


// linearly blends white and blue depending on the up/downess of the y coordinate
// essentially lerp between blue and white with clamped values: blended_value = (1-t)*start_value + t*end_value
vec3 colour(const ray& r, hitable *world, int depth)
{
	hit_record rec;

	// 2nd parameter is epsilon to reduce rounding errors.
	if (world->hit(r, 0.001, numeric_limits<float>::max(), rec))
	{
		ray scattered;
		vec3 attenuation;
		if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered))
		{
			return attenuation * colour(scattered, world, depth + 1);
		}
		else
		{
			return vec3(0);
		}

	}
	else
	{
		float t = 0.5f*(r.direction().y + 1.0f);
		return (1.0f - t) * vec3(1.0) + t * vec3(0.5, 0.7, 1.0);
	}
}
//
//hitable *random_scene() {
//	int n = 500;
//	hitable **list = new hitable*[n + 1];
//	list[0] = new sphere(vec3(0, -1000, 0), 1000, new lambertian(vec3(0.5, 0.5, 0.5)));
//	int i = 1;
//	for (int a = -11; a < 11; a++) {
//		for (int b = -11; b < 11; b++) {
//			float choose_mat = Utils::unif(Utils::rng);
//			vec3 center(a + 0.9*Utils::unif(Utils::rng), 0.2, b + 0.9*Utils::unif(Utils::rng));
//			if ((center - vec3(4, 0.2, 0)).length() > 0.9) {
//				if (choose_mat < 0.8) {  // diffuse
//					list[i++] = new sphere(center, 0.2, new lambertian(vec3(Utils::unif(Utils::rng)*Utils::unif(Utils::rng), Utils::unif(Utils::rng)*Utils::unif(Utils::rng), Utils::unif(Utils::rng)*Utils::unif(Utils::rng))));
//				}
//				else if (choose_mat < 0.95) { // metal
//					list[i++] = new sphere(center, 0.2,
//						new metal(vec3(0.5*(1 + Utils::unif(Utils::rng)), 0.5*(1 + Utils::unif(Utils::rng)), 0.5*(1 + Utils::unif(Utils::rng))), 0.5*Utils::unif(Utils::rng)));
//				}
//				else {  // glass
//					list[i++] = new sphere(center, 0.2, new dielectric(1.5));
//				}
//			}
//		}
//	}
//
//	list[i++] = new sphere(vec3(0, 1, 0), 1.0, new dielectric(1.5));
//	list[i++] = new sphere(vec3(-4, 1, 0), 1.0, new lambertian(vec3(0.4, 0.2, 0.1)));
//	list[i++] = new sphere(vec3(4, 1, 0), 1.0, new metal(vec3(0.7, 0.6, 0.5), 0.0));
//
//	return new hitable_list(list, i);
//}

int main()
{
	int nx = 800;
	int ny = 600;
	int ns = 50;
	ofstream ss("output.ppm");
	if (!ss.is_open())
		return 1;


	ss << "P3\n" << nx << " " << ny << " \n255\n";

	vec3 lookfrom = vec3(3, 3, 2);
	vec3 lookat(0, 0, -1);
	float dist = (lookfrom - lookat).length();

	camera* cam = new static_cam(lookfrom, lookat, vec3(0, 1, 0), 20, float(nx) / float(ny));
	//camera* cam = new dof_cam(lookfrom, lookat, vec3(0, 1, 0), 20, float(nx) / float(ny), 2.0, dist);

	hitable *list[5];

	list[0] = new sphere(vec3(0, 0, -1), 0.5, new lambertian(vec3(0.1, 0.2, 0.5)));
	list[1] = new sphere(vec3(0, -100.5, -1), 100, new lambertian(vec3(0.8, 0.8, 0.0)));
	list[2] = new sphere(vec3(1, 0, -1), 0.5, new metal(vec3(0.8, 0.6, 0.2), 0.0));
	list[3] = new sphere(vec3(-1, 0, -1), 0.5, new dielectric(1.5));
	list[4] = new sphere(vec3(-1, 0, -1), -0.45, new dielectric(1.5));
	hitable *world = new hitable_list(list, 5);
	
	//world = random_scene();

	// loop through each pixel
	for (int j = ny - 1; j >= 0; j--)
	{
		for (int i = 0; i < nx; i++)
		{
			vec3 col(0);

			// 1 distribution for each pixel
			std::uniform_real_distribution<double> unif(0, 1);
					   
			for (int s = 0; s < ns; s++)
			{
				// calculate uv coords
				float u = float(i + unif(Utils::rng)) / float(nx);
				float v = float(j + unif(Utils::rng)) / float(ny);

				ray r = cam->get_ray(u, v);
				vec3 p = r.hit_point(2.0);
				col += colour(r, world, 0);
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