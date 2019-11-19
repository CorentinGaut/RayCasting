#pragma once
#include "ray.h"

class camera {
public:
	camera() {
		lower_left_corner = Vec3<double>{ -2.0, -1.0, -1.0 };
		horizontal = Vec3<double>{ 4.0, 0.0, 0.0 };
		vertical = Vec3<double>{ 0.0, 2.0, 0.0 };
		origin = Vec3<double>{ 0.0, 0.0, 0.0 };
	}
	ray get_ray(float u, float v) {
		return ray(origin,
			lower_left_corner + u * horizontal + v * vertical - origin);
	}

	Vec3<double> origin;
	Vec3<double> lower_left_corner;
	Vec3<double> horizontal;
	Vec3<double> vertical;
};