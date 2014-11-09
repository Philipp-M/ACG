#include "GTriangle.hpp"
#include <cstdio>

Vec GTriangle::getNorm() const
{
	return normal;
}

Vec GTriangle::getColor() const
{
	return color;
}

Vec GTriangle::getEmission() const
{
	return emission;
}

Refl_t GTriangle::getReflectionType() const
{
	return refl;
}

bool GTriangle::intersect(const Ray &ray, RayIntPt& intPoint) const
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

	if(t <= 0)
		return false;
	intPoint.distance = t;
	intPoint.normal = normal;
	intPoint.position = ray.origin + ray.direction * t;
	intPoint.emission = emission;
	intPoint.reflType = refl;
	intPoint.color = color;

	return true;
}

Vec GTriangle::getCentroid() const
{
	return (v0 + v1 + v2)*0.33333333333333333333;
}

GTriangle::~GTriangle()
{
	// TODO Auto-generated destructor stub
}

GBoundingBox GTriangle::createBoundingBox() const
{
	Vec min, max;
//	min.x = v0.x <= v1.x ? v1.x <= v2.x ? v0.x : v0.x <= v2.x ? v0.x : v2.x : v1.x <= v2.x ? v1.x : v2.x;
//	min.y = v0.y <= v1.y ? v1.y <= v2.y ? v0.y : v0.y <= v2.y ? v0.y : v2.y : v1.y <= v2.y ? v1.y : v2.y;
//	min.z = v0.z <= v1.z ? v1.z <= v2.z ? v0.z : v0.z <= v2.z ? v0.z : v2.z : v1.z <= v2.z ? v1.z : v2.z;
//	had some fun with the above statements ;), but the straight forward solution is:
	min.x = std::min(std::min(v0.x,v1.x), v2.x);
	min.y = std::min(std::min(v0.y,v1.y), v2.y);
	min.z = std::min(std::min(v0.z,v1.z), v2.z);

	max.x = std::max(std::max(v0.x,v1.x), v2.x);
	max.y = std::max(std::max(v0.y,v1.y), v2.y);
	max.z = std::max(std::max(v0.z,v1.z), v2.z);

	return GBoundingBox(min, max);

}
