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

const GPrimitiveObject* GSphere::intersect(const Ray& ray, double& t) const
{
	return (t = intersect(ray)) != 0 ? this : NULL;
}

Vec GSphere::getCentroid() const
{
	return position;
}

GBoundingBox GSphere::createBoundingBox() const
{
	// todo
	return GBoundingBox(Vec(),Vec());
}
