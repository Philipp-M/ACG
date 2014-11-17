#include "GTexturedTriangle.hpp"

Vec3 GTexturedTriangle::getNorm() const
{
	return normal;
}

Vec3 GTexturedTriangle::getColor() const
{
	return color;
}

Vec3 GTexturedTriangle::getEmission() const
{
	return emission;
}

Refl_t GTexturedTriangle::getReflectionType() const
{
	return refl;
}
Vec3 GTexturedTriangle::getCentroid() const
{
	return (v0 + v1 + v2)*0.33333333333333333333;
}

bool GTexturedTriangle::intersect(const Ray& ray, RayIntPt& intPoint) const
{
}

GBoundingBox GTexturedTriangle::createBoundingBox() const
{
	Vec3 min, max;
	min.x = std::min(std::min(v0.x,v1.x), v2.x);
	min.y = std::min(std::min(v0.y,v1.y), v2.y);
	min.z = std::min(std::min(v0.z,v1.z), v2.z);

	max.x = std::max(std::max(v0.x,v1.x), v2.x);
	max.y = std::max(std::max(v0.y,v1.y), v2.y);
	max.z = std::max(std::max(v0.z,v1.z), v2.z);

	return GBoundingBox(min, max);

}
