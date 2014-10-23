#include "GTriangle.hpp"
#include <cstdio>

double GTriangle::intersect(const Ray& ray) const
{
	Vec edge1 = v1 - v0;
	Vec edge2 = v2 - v0;
	Vec dir = ray.direction;
	Vec orig = ray.origin;
	Vec pvec = dir%edge2;
	float det = edge1.dot(pvec);
	if (det == 0)
		return 0;
	float invDet = 1.0f / det;
	Vec tvec = orig - v0;
	double u = tvec.dot(pvec) * invDet;
	if (u < 0 || u > 1)
		return 0;
	Vec qvec = tvec%edge1;
	double v = dir.dot(qvec) * invDet;

	if (v < 0 || u + v > 1)
		return 0;
	double t = edge2.dot(qvec) * invDet;
	return t > 0 ? t : 0;
}

Vec GTriangle::getNorm(const Vec& position) const
{
	return normal;
}

Vec GTriangle::getColor(const Vec& position) const
{
	return color;
}

Vec GTriangle::getEmission(const Vec& position) const
{
	return emission;
}

Refl_t GTriangle::getReflectionType() const
{
	return refl;
}

const GPrimitiveObject* GTriangle::intersect(const Ray& ray, double& t) const
{
	return (t = intersect(ray)) != 0 ? this : NULL;
}

GTriangle::~GTriangle()
{
	// TODO Auto-generated destructor stub
}

