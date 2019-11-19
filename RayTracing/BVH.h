#pragma once
#include "Vec3.h"
#include "Ray.h"

inline double ffmin(double a, double b) { return a < b ? a : b; }
inline double ffmax(double a, double b) { return a > b ? a : b; }

class aabb {
public:
	aabb() {}
	aabb(const Vec3<double>& a, const Vec3<double>& b) { _min = a; _max = b; }

	Vec3<double> min() const { return _min; }
	Vec3<double> max() const { return _max; }

	bool hit(const ray& r, float tmin, float tmax) const {
		for (int a = 0; a < 3; a++) {
			double t0 = ffmin((_min.x - r.origin().x) / r.direction().x,
							(_max.x - r.origin().x) / r.direction().x);
			double t1 = ffmax((_min.x - r.origin().x) / r.direction().x,
							(_max.x - r.origin().x) / r.direction().x);
			tmin = ffmax(t0, tmin);
			tmax = ffmin(t1, tmax);
			if (tmax <= tmin)
				return false;
		}
		return true;
	}

	Vec3<double> _min;
	Vec3<double> _max;
};