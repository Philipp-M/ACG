#include "GSphere.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#ifdef __linux__
namespace glm
{
	typedef highp_mat4 mat4;
}
#endif


Vec3 GSphere::getNorm(const Vec3& position) const
{
	return (position - this->position).norm();
}

Vec3 GSphere::getColor() const
{
	return color;
}

Vec3 GSphere::getEmission() const
{
	return emission;
}

Refl_t GSphere::getReflectionType() const
{
	return refl;
}

bool GSphere::intersect(const Ray &ray, RayIntPt& intPoint) const
{
	Vec3 op = position - ray.origin; // Solve t^2*d.d + 2*t*(o-p).d + (o-p).(o-p)-R^2 = 0
	double t, eps = 1e-10, b = op.dot(ray.direction), det = b * b - op.dot(op) + radius * radius;
	if (det < 0)
		return 0;
	else
		det = sqrt(det);
	t = (t = b - det) > eps ? t : ((t = b + det) > eps ? t : 0);
	if(t == 0)
		return false;
	intPoint.distance = t;
	intPoint.position = ray.origin + ray.direction * t;
	intPoint.normal = (intPoint.position - position).norm();
	intPoint.calcNormal = intPoint.normal;
	intPoint.emission = emission;
	intPoint.reflType = refl;
	intPoint.color = color;

	return true;
}

Vec3 GSphere::getCentroid() const
{
	return position;
}

//simple translation operations
void GSphere::translate(const Vec3& t) {
	position = position + t;
}
void GSphere::rotationX(float rad) {
	rotation(rad, Vec3(1, 0, 0));
}
void GSphere::rotationY(float rad) {
	rotation(rad, Vec3(0, 1, 0));
}
void GSphere::rotationZ(float rad) {
	rotation(rad, Vec3(0, 0, 1));
}


void GSphere::rotation(float rad, const Vec3& dir) {
	glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(rad), glm::vec3(dir.x, dir.y, dir.z));
	
	glm::vec4  pos = glm::vec4(position.x, position.y, position.z, 1.0);
	
	pos = rotation * pos;

	position = Vec3(pos.x, pos.y, pos.z);
}

//very simple linear acceleration
void GSphere::translateAcc(Vec3 t, double acc, long time) {
	Vec3 v = Vec3(acc*time, acc*time, acc*time);

	t = t + v;	//simple linear accelaration wrt to time in seconds
	position = position + t;
}

GBoundingBox GSphere::createBoundingBox() const
{
	return GBoundingBox(Vec3(position.x-radius, position.y-radius, position.z-radius),
			Vec3(position.x+radius, position.y+radius, position.z+radius));
}
