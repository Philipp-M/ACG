#include "GPolygonObject.hpp"

GPolygonObject::GPolygonObject(const GPolygonObject &other) {
	std::vector<GTriangle> new_faces;
	for( GTriangle face : other.faces ) {
		GTriangle new_face = GTriangle(face);
		new_faces.push_back(new_face);
	}
	this->faces = new_faces;
	this->centroid = other.centroid;
	std::vector<GObject*> accelObjs;
	for( size_t i = 0; i < this->faces.size(); i++ )
		accelObjs.push_back(&this->faces[i]);
	this->accelStruct = new GBVHAccelStruct(accelObjs);
}



bool GPolygonObject::intersect(const Ray &ray, RayIntPt& intPoint) const
{
	return accelStruct->intersect(ray, intPoint);
}

Vec3 GPolygonObject::getCentroid() const
{
	return centroid;
}

//simple translation operatoins
void GPolygonObject::translate(const Vec3& t) {

	for( int i = 0; i < faces.size(); i++ )
		faces[i].translate(t);
	updateGeometry();
	
}
void GPolygonObject::rotationX(float rad) {
	for( int i = 0; i < faces.size(); i++ )
		faces[i].rotationCentroid(centroid, rad, Vec3(1, 0, 0));
	updateGeometry();
}

void GPolygonObject::rotationY(float rad) {
	for( int i = 0; i < faces.size(); i++ )
		faces[i].rotationCentroid(centroid, rad, Vec3(0, 1, 0));
	updateGeometry();
}

void GPolygonObject::rotationZ(float rad) {
	for( int i = 0; i < faces.size(); i++ )
		faces[i].rotationCentroid(centroid, rad, Vec3(0, 0, 1));
	updateGeometry();
}

//very simple liner acceleration
void GPolygonObject::translateAcc(Vec3 t, double acc, long time) {
	for( int i = 0; i < faces.size(); i++ )
		faces[i].translateAcc(t, acc, time);
	updateGeometry();
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
