#pragma once
#include "Vec3.h"

class ray
{
public:
	ray() {}

	ray(const Vec3<double>& a, const Vec3<double>& b) { A = a; B = b; }
	Vec3<double> origin() const { return A; }
	Vec3<double> direction() const { return B; }
	//retourne la position ou le rayon à touché
	Vec3<double> point_at_parameter(double t)  { return A + (t * B); }

	Vec3<double> A;
	Vec3<double> B;

};

