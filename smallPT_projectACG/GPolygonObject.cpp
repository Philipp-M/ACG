#include "GPolygonObject.hpp"

const GPrimitiveObject* GPolygonObject::intersect(const Ray& ray, double& t) const
{
	return accelStruct->intersect(ray, t);
}

Vec GPolygonObject::getCentroid() const
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
		return GBoundingBox(Vec(),Vec());
}
