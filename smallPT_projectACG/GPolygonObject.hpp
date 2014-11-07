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
	GPolygonObject(const GPolygonObject &other); // TODO: implement the constructor

	/***** implementations of GObject *****/
	bool intersect(const Ray &ray, RayIntPt& intPoint) const;
	Vec getCentroid() const;
	GBoundingBox createBoundingBox() const;
	/***** custom methods *****/
	void addFace(const GTriangle& triangle) { faces.push_back(triangle); updateGeometry(); }
	std::vector<GTriangle> getFaces() { return faces; }

	//simple translation operatoins
	void translate(const Vec& t);
	void rotationX(float rad);
	void rotationY(float rad);
	void rotationZ(float rad);

	//very simple liner acceleration
	void translateAcc(Vec t, double acc, long time);

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
		accelStruct = new GBVHAccelStruct(accelObjs);
		GBVHAccelStruct bvh(accelObjs);
	}

	void rotation(float rad, Vec dir);

	std::vector<GTriangle> faces;
	std::vector<GObject*> accelObjs; // temporarily for the simple accel Struct...
	Vec centroid;
	GAccelStruct* accelStruct;

};

#endif /* GPOLYGONOBJECT_HPP_ */
