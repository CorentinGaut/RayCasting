#pragma once

#include "hittable.h"

class sphere : public hittable {
public:
	sphere() {}
	sphere(Vec3<double> cen, float r, material* m)
		: center(cen), radius(r), mat_ptr(m) {};
	virtual bool hit(const ray& r, double tmin, double tmax, hit_record& rec) const;
	Vec3<double> center;
	float radius;
	material* mat_ptr; /* NEW */
};

bool sphere::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
	Vec3<double> oc = r.origin() - center;
	double a = dot(r.direction(), r.direction());
	double b = dot(oc, r.direction());
	double c = dot(oc, oc) - radius * radius;
	double discriminant = b * b - a * c;
	if (discriminant > 0) {
		float temp = (-b - sqrt(discriminant)) / a;
		if (temp < t_max && temp > t_min) {
			rec.t = temp;
			rec.p = r.A + rec.t * r.B;
			rec.normal = (rec.p - center) / radius;
			rec.mat_ptr = mat_ptr;
			return true;
		}
		temp = (-b + sqrt(discriminant)) / a;
		if (temp < t_max && temp > t_min) {
			rec.t = temp;
			rec.p = r.A + rec.t * r.B;
			rec.normal = (rec.p - center) / radius;
			rec.mat_ptr = mat_ptr;
			return true;
		}
	}
	return false;
}
