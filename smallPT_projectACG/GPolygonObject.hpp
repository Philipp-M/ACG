#ifndef GPOLYGONOBJECT_HPP_
#define GPOLYGONOBJECT_HPP_

#include <vector>
#include <cstdlib>
#include "GObject.hpp"
#include "GTriangle.hpp"
#include "GAccelStruct.hpp"
#include "GSimpleAccelStruct.hpp"
#include "GBVHAccelStruct.hpp"

template <class Triangle>
class GPolygonObject : public GObject
{
public:
	GPolygonObject() : accelStruct(NULL) { updateGeometry(); }
	GPolygonObject(const std::vector<Triangle>& triangles) : faces(triangles), accelStruct(NULL) { updateGeometry(); }
	GPolygonObject(const GPolygonObject<Triangle> &other)
	{
		faces = other.faces; // does not work for unknown reason...
		centroid = other.centroid;
		std::vector<GObject*> accelObjs;
		for (size_t i = 0; i < faces.size(); i++)
			accelObjs.push_back(&faces[i]);
		accelStruct = new GBVHAccelStruct(accelObjs);
	}

	/***** implementations of GObject *****/
	bool intersect(const Ray &ray, RayIntPt& intPoint) const;
	Vec3 getCentroid() const;
	GBoundingBox createBoundingBox() const;
	/***** custom methods *****/
	void addFace(const Triangle& triangle) { faces.push_back(triangle); updateGeometry(); }
	std::vector<Triangle> getFaces() { return faces; }

	~GPolygonObject() { }
private:
	void updateGeometry()
	{
		Vec3 tmp;
		std::vector<GObject*> accelObjs;
		for (size_t i = 0; i < faces.size(); i++)
			tmp = tmp + faces[i].getCentroid();
		centroid = tmp * (1.0 / faces.size());
		for (size_t i = 0; i < faces.size(); i++)
			accelObjs.push_back((GObject*) &faces[i]);
		// for now just the simple AccelStruct...
		if (accelStruct != NULL)
			delete accelStruct;
		accelStruct = new GBVHAccelStruct(accelObjs);
	}
	std::vector<Triangle> faces;
	Vec3 centroid;
	GAccelStruct* accelStruct;

};
template <class Triangle>
bool GPolygonObject<Triangle>::intersect(const Ray &ray, RayIntPt& intPoint) const
{
	return accelStruct->intersect(ray, intPoint);
}

template <class Triangle>
Vec3 GPolygonObject<Triangle>::getCentroid() const
{
	return centroid;
}
template <class Triangle>
GBoundingBox GPolygonObject<Triangle>::createBoundingBox() const
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

#endif /* GPOLYGONOBJECT_HPP_ */
