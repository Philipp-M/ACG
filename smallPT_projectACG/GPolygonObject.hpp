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

	/***** implementations of GObject *****/
	const GPrimitiveObject* intersect(const Ray& ray, double& t) const;
	Vec getCentroid() const;
	GBoundingBox createBoundingBox() const;
	/***** custom methods *****/
	void addFace(const GTriangle& triangle) { faces.push_back(triangle); updateGeometry(); }
	std::vector<GTriangle> getFaces() { return faces; }

	~GPolygonObject() { }
private:
	void updateGeometry()
	{
		Vec tmp;
		for (size_t i = 0; i < faces.size(); i++)
			tmp = tmp + faces[i].getCentroid();
		centroid = tmp * (1.0 / faces.size());
		accelObjs.clear();
		for (size_t i = 0; i < faces.size(); i++)
			accelObjs.push_back((GObject*) &faces[i]);
		// for now just the simple AccelStruct...
		if (accelStruct != NULL)
			delete accelStruct;
		accelStruct = new GSimpleAccelStruct(accelObjs);
	}
	std::vector<GTriangle> faces;
	std::vector<GObject*> accelObjs; // temporarily for the simple accel Struct...
	Vec centroid;
	GAccelStruct* accelStruct;

};

#endif /* GPOLYGONOBJECT_HPP_ */
