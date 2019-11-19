#include "pch.h"
#include "sphere.h"
#include "hittable_list.h"
#include "camera.h"
#include "lambertian.h"
#include <iostream>
#include <fstream>

using namespace winrt;
using namespace Windows::Foundation;


Vec3<double> color(const ray& r, hittable* world, int depth) {
	hit_record rec;
	if (world->hit(r, 0.001, INT16_MAX, rec)) {
		ray scattered;
		Vec3<double> attenuation;
		if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
			return attenuation * color(scattered, world, depth + 1);
		}
		else {
			return Vec3<double>{0, 0, 0};
		}
	}
	else {
		Vec3<double> unit_direction = unit_vector(r.direction());
		double t = 0.5 * (unit_direction.y + 1.0);
		return (1.0 - t) * Vec3<double> { 1.0, 1.0, 1.0 } +t * Vec3<double>{0.5, 0.7, 1.0};
	}
}

hittable* random_scene() {
	int n = 500;
	hittable** list = new hittable * [n + 1];
	list[0] = new sphere(Vec3<double>{0, -1000, 0}, 1000, new lambertian(Vec3<double>{0.5, 0.5, 0.5}));
	int i = 1;
	for (int a = -11; a < 11; a++) {
		for (int b = -11; b < 11; b++) {
			float choose_mat = random_double();
			Vec3<double> center{ a + 0.9 * random_double(), 0.2, b + 0.9 * random_double() };
			if (norm(center - Vec3<double>{4, 0.2, 0}) > 0.9) {
				if (choose_mat < 0.8) {  // diffuse
					list[i++] = new sphere(center, 0.2,
						new lambertian(Vec3<double>{random_double()* random_double(),
							random_double()* random_double(),
							random_double()* random_double()}
						)
					);
				}
				else if (choose_mat < 0.95) { // metal
					list[i++] = new sphere(center, 0.2,
						new metal(Vec3<double>{0.5* (1 + random_double()),
							0.5* (1 + random_double()),
							0.5* (1 + random_double())},
						0.5* random_double()));
				}
				else {  // glass
					list[i++] = new sphere(center, 0.2, new dielectric(1.5));
				}
			}
		}
	}

	list[i++] = new sphere(Vec3<double>{0, 1, 0}, 1.0, new dielectric(1.5));
	list[i++] = new sphere(Vec3<double>{-4, 1, 0}, 1.0, new lambertian(Vec3<double>{0.4, 0.2, 0.1}));
	list[i++] = new sphere(Vec3<double>{4, 1, 0}, 1.0, new metal(Vec3<double>{0.7, 0.6, 0.5}, 0.0));

	return new hittable_list(list, i);
}


int main()
{
	int nx = 1200;
	int ny = 600;
	int ns = 100;

	//fichier pgm
	std::ofstream out("out.ppm");
	out << "P3\n" << nx << ' ' << ny << ' ' << "255\n";

	//liste des objets
	/*hittable* list[5];
	list[0] = new sphere(Vec3<double>{0, 0, -5}, 0.5, new lambertian(Vec3<double>{0.8, 0.3, 0.3}));
	list[1] = new sphere(Vec3<double>{0, -100.5, -1}, 100, new lambertian(Vec3<double>{0.8, 0.8, 0.0}));
	list[2] = new sphere(Vec3<double>{1, 0, -1}, 0.5, new metal(Vec3<double>{0.8, 0.6, 0.2},1.0));
	list[3] = new sphere(Vec3<double>{-1, 0, -1}, 0.5, new dielectric(1.5));
	list[4] = new sphere(Vec3<double>{-1, 0, -1}, -0.45, new dielectric(1.5));*/



	hittable * world = random_scene();

	camera c;
	for (int j = ny - 1; j >= 0; j--) {
		for (int i = 0; i < nx; i++) {
			Vec3<double> col{ 0, 0, 0 };
			//anti aliasing qui rajoute un super effet
			for (int s = 0; s < ns; s++) {
				double u = double(i + random_double()) / double(nx);
				double v = double(j + random_double()) / double(ny);
				ray r = c.get_ray(u, v);
				col = color(r, world,0) + col;
			}
			col = col / double(ns);
			col = Vec3<double>{ sqrt(col.x), sqrt(col.y), sqrt(col.z) };

			//colorie dans le fichier pgm
			int ir = int(255.99 * col.x);
			int ig = int(255.99 * col.y);
			int ib = int(255.99 * col.z);
			out << ir << " " << ig << " " << ib << "\n";
		}
	}
}
