#include "GTriangle.hpp"
#include <cstdio>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

double GTriangle::intersect(const Ray& ray) const
{
	Vec edge1 = v1 - v0;
	Vec edge2 = v2 - v0;
	Vec dir = ray.direction;
	Vec orig = ray.origin;
	Vec pvec = dir%edge2;
	float det = edge1.dot(pvec);
	if (det == 0)
		return 0;
	float invDet = 1.0f / det;
	Vec tvec = orig - v0;
	double u = tvec.dot(pvec) * invDet;
	if (u < 0 || u > 1)
		return 0;
	Vec qvec = tvec%edge1;
	double v = dir.dot(qvec) * invDet;

	if (v < 0 || u + v > 1)
		return 0;
	double t = edge2.dot(qvec) * invDet;
	return t > 0 ? t : 0;
}

Vec GTriangle::getNorm(const Vec& position) const
{
	return normal;
}

Vec GTriangle::getColor(const Vec& position) const
{
	return color;
}

Vec GTriangle::getEmission(const Vec& position) const
{
	return emission;
}

Refl_t GTriangle::getReflectionType() const
{
	return refl;
}

const GPrimitiveObject* GTriangle::intersect(const Ray& ray, double& t) const
{
	return (t = intersect(ray)) != 0 ? this : NULL;
}

Vec GTriangle::getCentroid() const
{
	return (v0 + v1 + v2)*0.33333333333333333333;
}


void GTriangle::translate(Vec t) {
	v0 = v0 + t;  
	v1 = v1 + t; 
	v2 = v2 + t;
}

void GTriangle::rotationX(float rad) {
	rotation(rad, Vec(1, 0, 0));
}
void GTriangle::rotationY(float rad) {
	rotation(rad, Vec(0, 1, 0));
}
void GTriangle::rotationZ(float rad) {
	rotation(rad, Vec(0, 0, 1));
}

void GTriangle::rotation(float rad, Vec dir) {
	rotationCentroid(getCentroid(), rad,dir);
}

void rotationCentroid(Vec centroid, float rad, Vec dir) {

	glm::mat4 translateCentroid = glm::translate(glm::mat4(1.0), glm::vec3(centroid.x, centroid.y, centroid.z));
	glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(rad), glm::vec3(dir.x, dir.y, dir.z));
	glm::mat4 translateCentroidBack = glm::translate(glm::mat4(1.0), -glm::vec3(centroid.x, centroid.y, centroid.z));

	//not sure about the ordering, might be wrong, easy to test though
	rotation = translateCentroidBack * rotation * translateCentroid;

	glm::vec4  vec0 = glm::vec4(v0.x, v0.y, v0.z, 1.0);
	glm::vec4  vec1 = glm::vec4(v1.x, v1.y, v1.z, 1.0);
	glm::vec4  vec2 = glm::vec4(v2.x, v2.y, v2.z, 1.0);

	vec0 = rotation * vec0;
	vec1 = rotation * vec1;
	vec2 = rotation * vec2;

	v0 = Vec(vec0.x, vec0.y, vec0.z);
	v1 = Vec(vec1.x, vec1.y, vec1.z);
	v2 = Vec(vec2.x, vec2.y, vec2.z);
}

void GTriangle::translateAcc(Vec t, double acc, long time) {
	Vec v = Vec(acc*time, acc*time, acc*time);

	Vec t = t + v;	//simple linear accelaration wrt to time in seconds

	v0 = v0 + t;
	v1 = v1 + t;
	v2 = v2 + t;
}

GTriangle::~GTriangle()
{
	// TODO Auto-generated destructor stub
}

