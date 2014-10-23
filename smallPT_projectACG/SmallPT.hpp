
#ifndef SMALLPT_HPP_
#define SMALLPT_HPP_

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include "DataTypes.hpp"
#include "GScene.hpp"

class SmallPT
{
public:
	SmallPT();
	~SmallPT();

	static double clamp(double x)
	{
		return x < 0 ? 0 : x > 1 ? 1 : x; // 0.00 ... 1.0
	}
	static int toInt(double x)
	{
		return int(pow(clamp(x), 1 / 2.2) * 255 + .5);
	}
//	inline bool intersect(const Ray &r, double &t, int &id)
//	{
//		double n = sizeof(spheres) / sizeof(Sphere), d, inf = t = 1e20;
//		for (int i = int(n); i--;)
//			if ((d = spheres[i].intersect(r)) && d < t)
//			{
//				t = d;
//				id = i;
//			}
//		return t < inf;
//	}
	static Vec radiance(const Ray &ray, GScene* scene, int depth, unsigned short *Xi)
	{
		double t;                               // distance to intersection
		const GPrimitiveObject* intPoint = scene->intersect(ray, t);
		if (intPoint == NULL)
			return Vec(); // if miss, return black
		//const Sphere &object = spheres[id];        // the hit object
		Vec x = ray.origin + ray.direction * t;
		Vec n = intPoint->getNorm(x);
		Vec nl = n.dot(ray.direction) < 0 ? n : n * -1;
		Vec f = intPoint->getColor(x);
		Vec emission = intPoint->getEmission(x);
		Refl_t refl = intPoint->getReflectionType();
		if (++depth > 5)
		{
			double p = f.x > f.y && f.x > f.z ? f.x : f.y > f.z ? f.y : f.z; // max refl
			if (erand48(Xi) < p)
				f = f * (1 / p);
			else
				return emission; //R.R.
		}
		if (refl == DIFF)
		{                  // Ideal DIFFUSE reflection
			double r1 = 2 * M_PI * erand48(Xi), r2 = erand48(Xi), r2s = sqrt(r2);
			Vec w = nl, u = ((fabs(w.x) > .1 ? Vec(0, 1) : Vec(1)) % w).norm(), v = w % u;
			Vec d = (u * cos(r1) * r2s + v * sin(r1) * r2s + w * sqrt(1 - r2)).norm();
			return emission + f.mult(radiance(Ray(x, d), scene, depth, Xi));
		}
		else if (refl == SPEC)            // Ideal SPECULAR reflection
			return emission + f.mult(radiance(Ray(x, ray.direction - n * 2 * n.dot(ray.direction)), scene, depth, Xi));
		Ray reflRay(x, ray.direction - n * 2 * n.dot(ray.direction));     // Ideal dielectric REFRACTION
		bool into = n.dot(nl) > 0;                // Ray from outside going in?
		double nc = 1, nt = 1.5, nnt = into ? nc / nt : nt / nc, ddn = ray.direction.dot(nl), cos2t;
		if ((cos2t = 1 - nnt * nnt * (1 - ddn * ddn)) < 0)    // Total internal reflection
			return intPoint->getEmission(x) + f.mult(radiance(reflRay, scene, depth, Xi));

		Vec tdir = (ray.direction * nnt - n * ((into ? 1 : -1) * (ddn * nnt + sqrt(cos2t)))).norm();
		double a = nt - nc, b = nt + nc, R0 = a * a / (b * b), c = 1 - (into ? -ddn : tdir.dot(n));
		double Re = R0 + (1 - R0) * c * c * c * c * c, Tr = 1 - Re, P = .25 + .5 * Re, RP = Re / P, TP = Tr / (1 - P);
		return emission + f.mult(depth > 2 ? (erand48(Xi) < P ?   // Russian roulette
		radiance(reflRay, scene, depth, Xi) * RP : radiance(Ray(x, tdir), scene, depth, Xi) * TP) :
																radiance(reflRay, scene, depth, Xi) * Re + radiance(Ray(x, tdir), scene, depth, Xi) * Tr);
	}
};

#endif /* SMALLPT_HPP_ */
