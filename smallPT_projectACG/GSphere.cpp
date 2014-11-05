#include "GSphere.hpp"

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

bool GSphere::intersect(const Ray &ray, RayIntPt& intPoint) const
{
	double t = intersect(ray);
	if(t == 0)
		return false;
	intPoint.distance = t;
	intPoint.position = ray.origin + ray.direction * t;
	intPoint.normal = (intPoint.position - position).norm();
	intPoint.emission = emission;
	intPoint.reflType = refl;
	intPoint.color = color;

	return true;
}

Vec GSphere::getCentroid() const
{
	return position;
}
