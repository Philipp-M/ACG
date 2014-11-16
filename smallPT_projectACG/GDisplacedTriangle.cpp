#include "GDisplacedTriangle.hpp"

inline bool triangleIntersect(const Ray& ray, const Vec3& v0,const Vec3& v1,const Vec3& v2)
{
	Vec3 edge1 = v1 - v0;
	Vec3 edge2 = v2 - v0;
	Vec3 dir = ray.direction;
	Vec3 orig = ray.origin;
	Vec3 pvec = dir%edge2;
	float det = edge1.dot(pvec);
	if (det == 0)
		return false;
	float invDet = 1.0f / det;
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
	return true;
}
bool GDisplacedTriangle::intersect(const Ray& ray, RayIntPt& intPoint) const
{
	GBoundingBox bbox = createBoundingBox();
	double tFar,tNear;
	if(!bbox.intersect(ray,tNear,tFar))
		return false;

	return true;

}

Vec3 GDisplacedTriangle::getCentroid() const
{
	return (v0 * 2.0 + v1 * 2.0 + v2 * 2.0 + normal * 3 * maxHeight) * 0.166666666666666666;
}

GBoundingBox GDisplacedTriangle::createBoundingBox() const
{
	Vec3 min1, min2, max1, max2;
	Vec3 v0d = v0 + normal * maxHeight;
	Vec3 v1d = v1 + normal * maxHeight;
	Vec3 v2d = v2 + normal * maxHeight;
	min1.x = std::min(std::min(v0.x, v1.x), v2.x);
	min1.y = std::min(std::min(v0.y, v1.y), v2.y);
	min1.z = std::min(std::min(v0.z, v1.z), v2.z);

	max1.x = std::max(std::max(v0.x, v1.x), v2.x);
	max1.y = std::max(std::max(v0.y, v1.y), v2.y);
	max1.z = std::max(std::max(v0.z, v1.z), v2.z);

	min2.x = std::min(std::min(v0d.x, v1d.x), v2d.x);
	min2.y = std::min(std::min(v0d.y, v1d.y), v2d.y);
	min2.z = std::min(std::min(v0d.z, v1d.z), v2d.z);

	max2.x = std::max(std::max(v0d.x, v1d.x), v2d.x);
	max2.y = std::max(std::max(v0d.y, v1d.y), v2d.y);
	max2.z = std::max(std::max(v0d.z, v1d.z), v2d.z);
	return GBoundingBox(min1, max1) + GBoundingBox(min2, max2);
}
