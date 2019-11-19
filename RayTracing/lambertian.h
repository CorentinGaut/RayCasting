#pragma once
#include "hittable.h"
class lambertian : public material {
public:
	lambertian(const Vec3<double>& a) : albedo(a) {}
	virtual bool scatter(const ray& r_in, const hit_record& rec, Vec3<double>& attenuation, ray& scattered) const {
		Vec3<double> target = rec.p + rec.normal + random_in_unit_sphere();
		scattered = ray(rec.p, target - rec.p);
		attenuation = albedo;
		return true;
	}

	Vec3<double> albedo;
};

class metal : public material {
public:
	metal(const Vec3<double>& a, double f) : albedo(a) {
		if (f < 1) fuzz = f; else fuzz = 1;
	}
	virtual bool scatter(const ray& r_in, const hit_record& rec, Vec3<double>& attenuation, ray& scattered) const {
		Vec3<double> reflected = reflect(unit_vector(r_in.direction()), rec.normal);
		//ajout du fuzz au rayon
		scattered = ray(rec.p, reflected + fuzz * random_in_unit_sphere());
		attenuation = albedo;
		return (dot(scattered.direction(), rec.normal) > 0);
	}
	Vec3<double> albedo;
	double fuzz;
};

class dielectric : public material {
public:
	dielectric(double ri) : ref_idx(ri) {}
	virtual bool scatter(const ray& r_in, const hit_record& rec,
		Vec3<double>& attenuation, ray& scattered) const {
		Vec3<double> outward_normal;
		Vec3<double> reflected = reflect(r_in.direction(), rec.normal);
		float ni_over_nt;
		attenuation = Vec3<double>{ 1.0, 1.0, 1.0 };
		Vec3<double> refracted;

		float reflect_prob;
		float cosine;

		if (dot(r_in.direction(), rec.normal) > 0) {
			outward_normal = -1.0 * rec.normal;
			ni_over_nt = ref_idx;
			cosine = ref_idx * dot(r_in.direction(), rec.normal) / norm(r_in.direction());
		}
		else {
			outward_normal = rec.normal;
			ni_over_nt = 1.0 / ref_idx;
			cosine = -dot(r_in.direction(), rec.normal) / norm(r_in.direction());
		}

		if (refract(r_in.direction(), outward_normal, ni_over_nt, refracted)) {
			reflect_prob = schlick(cosine, ref_idx);
		}
		else {
			reflect_prob = 1.0;
		}

		if (random_double() < reflect_prob) {
			scattered = ray(rec.p, reflected);
		}
		else {
			scattered = ray(rec.p, refracted);
		}

		return true;
	}

	double ref_idx;
};