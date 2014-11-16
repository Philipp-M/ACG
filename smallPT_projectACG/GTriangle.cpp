#include "GTriangle.hpp"
#include <cstdio>

Vec3 GTriangle::getNorm() const
{
	return normal;
}

Vec3 GTriangle::getColor() const
{
	return color;
}

Vec3 GTriangle::getEmission() const
{
	return emission;
}

Refl_t GTriangle::getReflectionType() const
{
	return refl;
}

bool GTriangle::intersect(const Ray &ray, RayIntPt& intPoint) const
{
	Vec3 edge1 = v1 - v0;
	Vec3 edge2 = v2 - v0;
	Vec3 dir = ray.direction;
	Vec3 orig = ray.origin;
	Vec3 pvec = dir%edge2;
	double det = edge1.dot(pvec);
	if (det == 0)
		return false;
	double invDet = 1.0f / det;
	Vec3 tvec = orig - v0;
	double u = tvec.dot(pvec) * invDet;
	if (u < 0 || u > 1)
		return false;
	Vec3 qvec = tvec%edge1;
	double v = dir.dot(qvec) * invDet;

	if (v < 0 || u + v > 1)
		return false;
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

Vec3 GTriangle::getCentroid() const
{
	return (v0 + v1 + v2)*0.33333333333333333333;
}

GTriangle::~GTriangle()
{
	// TODO Auto-generated destructor stub
}

GBoundingBox GTriangle::createBoundingBox() const
{
	Vec3 min, max;
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
