
#ifndef SMALLPT_HPP
#define SMALLPT_HPP
#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include "DataTypes.hpp"
#include "GScene.hpp"


#ifdef _WIN32
double inline erand48(unsigned short xsubi[3]) {
	return ((double)rand() / (double)RAND_MAX);
}
#endif

class SmallPT
{
public:
	SmallPT() {};
	~SmallPT() {};

	static double clamp(double x)
	{
		return x < 0 ? 0 : x > 1 ? 1 : x; // 0.00 ... 1.0
	}
	static int toInt(double x)
	{
		return int(pow(clamp(x), 1 / 2.2) * 255 + .5);
	}
	static Vec3 radiance(const Ray &ray, GScene* scene, int depth, unsigned short *Xi)
	{
		/**
		 * use the newly introduced intersection Point structure
		 */
		RayIntPt intPoint;
		if (!scene->intersect(ray, intPoint))
			return Vec3(); // if miss, return black
		// for speed purpose, if the surface fully absorbs, stop the recursion
		if(intPoint.color.x == 0 && intPoint.color.y == 0 && intPoint.color.z == 0 &&
			intPoint.emission.x == 0 && intPoint.emission.y == 0 && intPoint.emission.z == 0)
			return Vec3(); // if completely absorbs, return black
		/**
		 * calculate the normal where there is a sharp angle between the normal and the ray
		 */
		Vec3 nl = intPoint.normal.dot(ray.direction) < 0 ? intPoint.normal : intPoint.normal * -1;
		Vec3 ncl = intPoint.calcNormal.dot(ray.direction) < 0 ? intPoint.calcNormal : intPoint.calcNormal * -1;

		// stop when there is light
		if(intPoint.emission.x > 0 || intPoint.emission.y > 0 || intPoint.emission.z > 0)
			return intPoint.emission;

		if (++depth > 5)
		{
			double p = intPoint.color.x > intPoint.color.y && intPoint.color.x > intPoint.color.z ? intPoint.color.x : intPoint.color.y > intPoint.color.z ? intPoint.color.y : intPoint.color.z; // max refl
			if (erand48(Xi) < p)
				intPoint.color = intPoint.color * (1 / p);
			else
				return intPoint.emission; // Russian Roulette
		}
		if (intPoint.reflType == DIFF)   // Ideal DIFFUSE reflection
		{
			double r1 = 2 * M_PI * erand48(Xi), r2 = erand48(Xi), r2s = sqrt(r2);
			Vec3 w = ncl, u = ((fabs(w.x) > .1 ? Vec3(0, 1) : Vec3(1)) % w).norm(), v = w % u;
			Vec3 d = (u * cos(r1) * r2s + v * sin(r1) * r2s + w * sqrt(1 - r2)).norm();
			double orient = nl.dot(d);
			// following trick is necessary with calculated normals,
			// if the new direction points beneath the surface, reflect it around the calculated normal
			if(orient < 0)
				return intPoint.emission + intPoint.color.mult(radiance(Ray(intPoint.position, d - w * 2 * w.dot(d)), scene, depth, Xi));
			else
				return intPoint.emission + intPoint.color.mult(radiance(Ray(intPoint.position, d), scene, depth, Xi));
			//return intPoint.emission + intPoint.color.mult(radiance(Ray(intPoint.position, d), scene, depth, Xi));
		}
		else if (intPoint.reflType == GLOSS)            // GLOSSY reflection
		{
			// calculate a random ray around the circle(the size is defined by glossyRoughness),
			// where the reflected ray is the center
			double r1 = 2 * M_PI * erand48(Xi), r2 = intPoint.glossyRoughness * erand48(Xi), r2s = sqrt(r2);
			Vec3 w = ray.direction - intPoint.calcNormal * 2 * intPoint.calcNormal.dot(ray.direction);
			Vec3 u = ((fabs(w.x) > .1 ? Vec3(0, 1) : Vec3(1)) % w).norm(), v = w % u;
			Vec3 d = (u * cos(r1) * r2s + v * sin(r1) * r2s + w * sqrt(1 - r2)).norm();
			double orient = nl.dot(d);
			// following trick is necessary with calculated normals,
			// if the new direction points beneath the surface, reflect it around the calculated normal
			if(orient < 0)
				return intPoint.emission + intPoint.color.mult(radiance(Ray(intPoint.position, d - w * 2 * w.dot(d)), scene, depth, Xi));
			else
				return intPoint.emission + intPoint.color.mult(radiance(Ray(intPoint.position, d), scene, depth, Xi));
		}
		else if (intPoint.reflType == SPEC)            // Ideal SPECULAR reflection
			return intPoint.emission + intPoint.color.mult(radiance(Ray(intPoint.position, ray.direction - intPoint.normal * 2 * intPoint.normal.dot(ray.direction)), scene, depth, Xi));
		Ray reflRay(intPoint.position, ray.direction - intPoint.normal * 2 * intPoint.normal.dot(ray.direction));     // Ideal dielectric REFRACTION
		bool into = intPoint.normal.dot(nl) > 0;                // Ray from outside going in?
		double nc = 1, nt = 1.5, nnt = into ? nc / nt : nt / nc, ddn = ray.direction.dot(nl), cos2t;
		if ((cos2t = 1 - nnt * nnt * (1 - ddn * ddn)) < 0)    // Total internal reflection
			return intPoint.emission + intPoint.color.mult(radiance(reflRay, scene, depth, Xi));

		Vec3 tdir = (ray.direction * nnt - intPoint.normal * ((into ? 1 : -1) * (ddn * nnt + sqrt(cos2t)))).norm();
		double a = nt - nc, b = nt + nc, R0 = a * a / (b * b), c = 1 - (into ? -ddn : tdir.dot(intPoint.normal));
		double Re = R0 + (1 - R0) * c * c * c * c * c, Tr = 1 - Re, P = .25 + .5 * Re, RP = Re / P, TP = Tr / (1 - P);
		/**
		 * Beer-Lambert Law extra feature, simple but nice feature for colored glass
		 * 'theory' described in the documentation
		 */
		// if the ray is going into the medium, the (reflected) color will be white(mirror reflection)
		if(into)
			intPoint.color = Vec3(0.999,0.999,0.999); // usually white reflection, maybe a specular color will be implemented some time
		else
		{
			// else it is leaving the material,
			// here it gets interesting, the density Factor is just calculated out of the 'length' of the color
			double densityFactor = sqrt(intPoint.color.dot(intPoint.color));
			// then the absorbed color will be inverted( note the word absorbed ;) ) since if
			// red color is desired, the opposite has to be absorbed
			Vec3 fn = intPoint.color * (1.0/densityFactor); //intPoint.color.norm();
			// then finally the beer-lambert law is executed, on all color components
			intPoint.color = Vec3(exp(-intPoint.distance*densityFactor*(1-fn.x)),exp(-intPoint.distance*densityFactor*(1-fn.y)),exp(-intPoint.distance*densityFactor*(1-fn.z)));
		}
		return intPoint.emission + intPoint.color.mult(depth > 2 ? (erand48(Xi) < P ?   // Russian roulette
				radiance(reflRay, scene, depth, Xi) * RP : radiance(Ray(intPoint.position, tdir), scene, depth, Xi) * TP) :
																			radiance(reflRay, scene, depth, Xi) * Re + radiance(Ray(intPoint.position, tdir), scene, depth, Xi) * Tr);
	}
};

#endif //SMALLPT_HPP
