#pragma once

#include "ray.h"
#include "hitable.h"
#include <random>
#include <chrono>
#include <glm/gtx/norm.hpp>



class material
{
public:
	//std::mt19937_64 rng;
	//std::uniform_real_distribution<double> unif;
	virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const = 0;
	vec3 reflect(const vec3& v, const vec3& n) const;
	vec3 albedo;
};


// lambert diffuse reflection case
class lambertian : public material
{
public:
	lambertian(const vec3& a) { albedo = a; }
	virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const;

};

// metal materials
class metal : public material
{
public:
	metal(const vec3& a, float f)  
	{
		albedo = a;
		fuzz = (f < 1) ? f : 1;
	}
	virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const;
	float fuzz;
};


// dielectric material (always refracts when possible)
class dielectric : public material
{
public:
	dielectric(float ri) : ref_idx(ri) {}
	virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const;
	bool refract(const vec3& v, const vec3& n, float ni_over_nt, vec3& refracted) const;
	float ref_idx;
	float schlick(float cosine, float ref_idx) const;
};