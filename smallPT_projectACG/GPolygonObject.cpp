#include "GPolygonObject.hpp"

bool GPolygonObject::intersect(const Ray &ray, RayIntPt& intPoint) const
{
	return accelStruct->intersect(ray, intPoint);
}

Vec GPolygonObject::getCentroid() const
{
	return centroid;
}
