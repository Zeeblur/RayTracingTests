#pragma once

#include "hitable.h"

class sphere : public hitable {
public:
	sphere() {}
	sphere(vec3 cen, float r, material* matptr) : center(cen), radius(r), mat_ptr(matptr){};

	virtual bool hit(const ray& r, float tmin, float tmas, hit_record& rec) const;

	vec3 center; 
	float radius;
	material* mat_ptr;
};