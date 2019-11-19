#pragma once
#include <iostream>
#include <functional>
#include <random>

template<typename T>
struct Vec3 {
	T x, y, z;
};


//Operateur egalitaire
template<typename T>
bool operator==(const Vec3<T>& a, const Vec3<T>& b) {
	return a.x == b.x && a.y == b.y && a.z == b.z;
};

//Operateur multiplicateur entre 2 vec3
template<typename T>
Vec3<T> operator*(const Vec3<T>& a, const Vec3<T>& b) {
	return Vec3<T>{ a.x* b.x, a.y* b.y, a.z* b.z };
};

//Operateur multiplicateur avec un scalaire
template<typename T>
Vec3<T> operator*(const Vec3<T>& a, double b) {
	return Vec3<T>{ a.x* b, a.y* b, a.z* b };
};

//Operateur multiplicateur avec un scalaire dans l'autre sens
template<typename T>
Vec3<T> operator*(double b, const Vec3<T>& a) {
	return Vec3<T>{ b * a.x, b* a.y, b* a.z };
};

//Operateur diviseur entre 2 vec3 
template<typename T>
Vec3<T> operator/(const Vec3<T>& a, const Vec3<T>& b) {
	return Vec3<T>{ a.x / b.x, a.y / b.y, a.z / b.z };
};

//Operateur diviseur avec un scalaire
template<typename T>
Vec3<T> operator/(const Vec3<T> & a, double b) {
	return Vec3<T>{ a.x / b, a.y / b, a.z / b };
};

//Operateur diviseur avec un scalaire
template<typename T>
Vec3<T> operator/(double b, const Vec3<T> & a) {
	return Vec3<T>{ b / a.x, b / a.y, b / a.z };
};

//Operateur de soustraction
template<typename T>
Vec3<T> operator-(const Vec3<T> & a, const Vec3<T> & b) {
	return Vec3<T>{ a.x - b.x, a.y - b.y, a.z - b.z };
};

//Operateur de soustraction
template<typename T>
Vec3<T> operator-(const Vec3<T> & a, double b) {
	return Vec3<T>{ a.x - b, a.y - b, a.z - b };
};

//Operateur d'addition
template<typename T>
Vec3<T> operator+(const Vec3<T> & a, const Vec3<T> & b) {
	return Vec3<T>{ a.x + b.x, a.y + b.y, a.z + b.z };
};

//Operateur d'addition avec un scalaire
template<typename T>
Vec3<T> operator+(const Vec3<T> & a, double b) {
	return Vec3<T>{ a.x + b, a.y + b, a.z + b };
};

//Calcul de la nomre d'un vecteur
template<typename T>
double norm(const Vec3<T> & v) {
	return std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
};

template<typename T>
Vec3<T> normalize(const Vec3<T> & v) {
	double norme = norm(v);
	return Vec3<T>{ v.x / norme, v.y / norme, v.z / norme };
}

template<typename T>
double squareLength(const Vec3<T>& v) {
	return v.x * v.x + v.y * v.y + v.z * v.z;
};

//Calcul du produit scalaire
template<typename T>
double dot(const Vec3<T> & a, const Vec3<T> & b) {
	return a.x* b.x + a.y * b.y + a.z * b.z;
}

template<typename T>
double dot2(const Vec3<T> & a) {
	return norm(a)* norm(a);
}

template<typename T>
void clamp255(Vec3<T> & col) {
	col.x = (col.x > 255) ? 255 : (col.x < 0) ? 0 : col.x;
	col.y = (col.y > 255) ? 255 : (col.y < 0) ? 0 : col.y;
	col.z = (col.z > 255) ? 255 : (col.z < 0) ? 0 : col.z;
}

template<typename T>
void debugLog(const Vec3<T> & v) {
	std::cout << "x : " << v.x << " y : " << v.y << " z : " << v.z << std::endl;
}

inline float random_between_two_float(float min, float max)
{
	return (min + 1) + (((float)rand()) / (float)RAND_MAX) * (max - (min + 1));
}

inline Vec3<double> unit_vector(Vec3<double> v) {
	return v / norm(v);
}

inline double random_double() {
	static std::uniform_real_distribution<double> distribution(0.0, 1.0);
	static std::mt19937 generator;
	static std::function<double()> rand_generator = std::bind(distribution, generator);
	return rand_generator();
}

Vec3<double> random_in_unit_sphere() {
	Vec3<double> p;
	do {
		p = 2.0 * Vec3<double>{random_double(), random_double(), random_double()} -Vec3<double>{1, 1, 1};
	} while (squareLength(p) >= 1.0);
	return p;
}

template<typename T>
Vec3<T> reflect(const Vec3<T>& v, const Vec3<T>& n) {
	return v - 2 * dot(v, n) * n;
}

template<typename T>
bool refract(const Vec3<T>& v, const Vec3<T>& n, double ni_over_nt, Vec3<T>& refracted) {
	Vec3<T> uv = unit_vector(v);
	float dt = dot(uv, n);
	float discriminant = 1.0 - ni_over_nt * ni_over_nt * (1 - dt * dt);
	if (discriminant > 0) {
		refracted = ni_over_nt * (uv - n * dt) - n * sqrt(discriminant);
		return true;
	}
	else
		return false;
}

double schlick(double cosine, double ref_idx) {
	double r0 = (1 - ref_idx) / (1 + ref_idx);
	r0 = r0 * r0;
	return r0 + (1 - r0) * pow((1 - cosine), 5);
}


