#include "GSphere.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#ifdef __linux__
namespace glm
{
	typedef highp_mat4 mat4;
}
#endif

double GSphere::intersect(const Ray &ray) const
{
	Vec op = position - ray.origin; // Solve t^2*d.d + 2*t*(o-p).d + (o-p).(o-p)-R^2 = 0
	double t, eps = 1e-10, b = op.dot(ray.direction), det = b * b - op.dot(op) + radius * radius;
	if (det < 0)
		return 0;
	else
		det = sqrt(det);
	return (t = b - det) > eps ? t : ((t = b + det) > eps ? t : 0);
}

Vec GSphere::getNorm(const Vec& position) const
{
	return (position - this->position).norm();
}

Vec GSphere::getColor(const Vec& position) const
{
	return color;
}

Vec GSphere::getEmission(const Vec& position) const
{
	return emission;
}

Refl_t GSphere::getReflectionType() const
{
	return refl;
}

const GPrimitiveObject* GSphere::intersect(const Ray& ray, double& t) const
{
	return (t = intersect(ray)) != 0 ? this : NULL;
}

Vec GSphere::getCentroid() const
{
	return position;
}


//simple translation operatoins
void GSphere::translate(const Vec& t) {
	position = position + t;
}
void GSphere::rotationX(float rad) {
	rotation(rad, Vec(1, 0, 0));
}
void GSphere::rotationY(float rad) {
	rotation(rad, Vec(0, 1, 0));
}
void GSphere::rotationZ(float rad) {
	rotation(rad, Vec(0, 0, 1));
}


void GSphere::rotation(float rad, const Vec& dir) {
	glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(rad), glm::vec3(dir.x, dir.y, dir.z));
	
	glm::vec4  pos = glm::vec4(position.x, position.y, position.z, 1.0);
	
	pos = rotation * pos;

	position = Vec(pos.x, pos.y, pos.z);
}

//very simple liner acceleration
void GSphere::translateAcc(Vec t, double acc, long time) {
	Vec v = Vec(acc*time, acc*time, acc*time);

	t = t + v;	//simple linear accelaration wrt to time in seconds
	position = position + t;
}
