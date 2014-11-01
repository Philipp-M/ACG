#include "GPolygonObject.hpp"

const GPrimitiveObject* GPolygonObject::intersect(const Ray& ray, double& t) const
{
	return accelStruct->intersect(ray, t);
}

Vec GPolygonObject::getCentroid() const
{
	return centroid;
}
