#include "GTexturedTriangle.hpp"

Vec3 GTexturedTriangle::getNorm() const
{
	return normal;
}

Vec3 GTexturedTriangle::getColor() const
{
	return color;
}

Vec3 GTexturedTriangle::getEmission() const
{
	return emission;
}

Refl_t GTexturedTriangle::getReflectionType() const
{
	return refl;
}
Vec3 GTexturedTriangle::getCentroid() const
{
	return (v0 + v1 + v2)*0.33333333333333333333;
}

bool GTexturedTriangle::intersect(const Ray& ray, RayIntPt& intPoint) const
{
	Vec3 edge1 = v1 - v0;
	Vec3 edge2 = v2 - v0;
	Vec3 dir = ray.direction;
	Vec3 orig = ray.origin;
	Vec3 pvec = dir % edge2;
	double det = edge1.dot(pvec);
	if (det == 0)
		return false;
	double invDet = 1.0 / det;
	Vec3 tvec = orig - v0;
	double u = tvec.dot(pvec) * invDet;
	if (u < 0 || u > 1)
		return false;
	Vec3 qvec = tvec % edge1;
	double v = dir.dot(qvec) * invDet;

	if (v < 0 || u + v > 1)
		return false;
	double t = edge2.dot(qvec) * invDet;

	if (t <= 0.00000001)
		return false;
	Vec2 texCoordinate = (wt + (ut - wt) * u + (vt - wt) * v).mult(Vec2(colorMap->width, colorMap->height)); // normal and specular map should hold the same width/height
	const uint8_t* pixel = &colorMap->pixels[texCoordinate.y*colorMap->width+texCoordinate.x];
	intPoint.color = Vec3(pixel[0]*1.0/255.0*0.999, pixel[1]*1.0/255.0*0.999, pixel[2]*1.0/255.0*0.999);

	// todo if(normalMap != NULL)

	if(specularMap != NULL)
	{
		intPoint.glossyRoughness = 1.0-colorMap->pixels[texCoordinate.y*colorMap->width+texCoordinate.x]*1.0/255.0;
		if(intPoint.glossyRoughness >= 0.999)
			intPoint.reflType = DIFF;
		else
			intPoint.reflType = GLOSS;
	} else
		intPoint.reflType = DIFF;


}

GBoundingBox GTexturedTriangle::createBoundingBox() const
{
	Vec3 min, max;
	min.x = std::min(std::min(v0.x,v1.x), v2.x);
	min.y = std::min(std::min(v0.y,v1.y), v2.y);
	min.z = std::min(std::min(v0.z,v1.z), v2.z);

	max.x = std::max(std::max(v0.x,v1.x), v2.x);
	max.y = std::max(std::max(v0.y,v1.y), v2.y);
	max.z = std::max(std::max(v0.z,v1.z), v2.z);

	return GBoundingBox(min, max);

}
