#include "GPolygonObject.hpp"

const GPrimitiveObject* GPolygonObject::intersect(const Ray& ray, double& t) const
{
	return accelStruct->intersect(ray, t);
}

Vec GPolygonObject::getCentroid() const
{
	return centroid;
}

//simple translation operatoins
void GPolygonObject::translate(Vec t) {
	for( GTriangle f : faces ) 
		f.translate(t);
	updateGeometry();
	
}
void GPolygonObject::rotationX(float rad) {
	for( GTriangle f : faces )
		f.rotationCentroid(centroid, rad, Vec(1, 0, 0));
	updateGeometry();
}

void GPolygonObject::rotationY(float rad) {
	for( GTriangle f : faces )
		f.rotationCentroid(centroid, rad, Vec(0, 1, 0));
	updateGeometry();
}

void GPolygonObject::rotationZ(float rad) {
	for( GTriangle f : faces )
		f.rotationCentroid(centroid, rad, Vec(0, 0, 1));
	updateGeometry();
}

//very simple liner acceleration
void GPolygonObject::translateAcc(Vec t, double acc, long time) {
	for( GTriangle f : faces )
		f.translateAcc(t, acc, time);
	updateGeometry();
}
