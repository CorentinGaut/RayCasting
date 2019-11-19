#pragma once
#include "Ray.h"


class material;

struct hit_record {
	double t;
	Vec3<double> p;
	Vec3<double> normal;
	material* mat_ptr;
};

class hittable {
public:
	virtual bool hit(
		const ray& r, double t_min, double t_max, hit_record& rec) const = 0;
};

class material {
public:
	virtual bool scatter(const ray& r_in, const hit_record& rec, Vec3<double>& attenuation, ray& scattered) const = 0;
};
