#ifndef GPOLYGONOBJECT_HPP_
#define GPOLYGONOBJECT_HPP_

#include <vector>
#include <cstdlib>
#include "GObject.hpp"
#include "GTriangle.hpp"
#include "GAccelStruct.hpp"
#include "GSimpleAccelStruct.hpp"
#include "GBVHAccelStruct.hpp"

class GPolygonObject : public GObject
{
public:
	GPolygonObject() : accelStruct(NULL) { updateGeometry(); }
	GPolygonObject(const std::vector<GTriangle>& triangles) : faces(triangles), accelStruct(NULL) { updateGeometry(); }
	GPolygonObject(const GPolygonObject &other)
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
	void addFace(const GTriangle& triangle) { faces.push_back(triangle); updateGeometry(); }
	std::vector<GTriangle> getFaces() { return faces; }

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
	std::vector<GTriangle> faces;
	Vec3 centroid;
	GAccelStruct* accelStruct;

};

#endif /* GPOLYGONOBJECT_HPP_ */
