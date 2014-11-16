#include "GPolygonObject.hpp"

bool GPolygonObject::intersect(const Ray &ray, RayIntPt& intPoint) const
{
	return accelStruct->intersect(ray, intPoint);
}

Vec3 GPolygonObject::getCentroid() const
{
	return centroid;
}

GBoundingBox GPolygonObject::createBoundingBox() const
{
	if(!faces.empty())
	{
		GBoundingBox bbox = faces[0].createBoundingBox();
		for(size_t i = 1; i < faces.size(); i++)
			bbox = bbox + faces[i].createBoundingBox();
		return bbox;
	}
	else
		return GBoundingBox(Vec3(),Vec3());
}
