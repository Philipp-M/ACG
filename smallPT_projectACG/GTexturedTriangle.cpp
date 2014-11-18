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
	intPoint.distance = t;
	intPoint.normal = normal;
	intPoint.position = ray.origin + ray.direction * t;
	intPoint.emission = emission;
	intPoint.reflType = refl;
	intPoint.color = color;
	if (colorMap != NULL || normalMap != NULL || specularMap != NULL)
	{
		double w = 1.0 - u - v;
		Vec2 texCoordinate = ut * w + vt * u + wt * v;
		if (colorMap != NULL)
		{
			size_t pixelIndex = (size_t) (colorMap->width * (colorMap->height - 1) - ((int) (texCoordinate.y * colorMap->height - 0.00001)) * colorMap->width
					+ ((int) (texCoordinate.x * colorMap->width - 0.00001))) * 4;
			const uint8_t* pixel = &colorMap->pixels[pixelIndex];
			intPoint.color = color.mult(
					Vec3(((double) pixel[0]) * 1.0 / 255.0 * 0.999, ((double) pixel[1]) * 1.0 / 255.0 * 0.999, ((double) pixel[2]) * 1.0 / 255.0 * 0.999));
		}
		if (normalMap != NULL)
		{
			// calculate normal of the map
			size_t pixelIndex = (size_t) (specularMap->width * (specularMap->height - 1)
					- ((int) (texCoordinate.y * specularMap->height - 0.00001)) * specularMap->width + ((int) (texCoordinate.x * specularMap->width - 0.00001)))
					* 4;
			const uint8_t* pixel = &normalMap->pixels[pixelIndex];
			Vec3 texNormal = Vec3(1.0 / 128 * pixel[0] - 1, 1.0 / 128 * pixel[1] - 1, 1.0 / 128 * pixel[2] - 1).norm();
			Vec3 dv01 = v1 - v0;
			Vec3 dv02 = v2 - v0;
			Vec2 duv01 = vt - ut;
			Vec2 duv02 = vt - ut;
			double r = 1.0 / (duv01.x * duv02.y - duv01.y * duv02.x);
			Vec3 tan = (dv01 * duv02.y - dv02 * duv01.y) * r;
			Vec3 btan = (dv02 * duv01.x - dv01 * duv02.x) * r;
			Vec3 calcNormal;
			calcNormal.x = tan.dot(texNormal);
			calcNormal.y = btan.dot(texNormal);
			calcNormal.z = normal.dot(texNormal);
			intPoint.normal = texNormal.norm();
			//std::cerr << "texNormal is: " << texNormal << " normal is: " << normal << " calculated normal is: " << calcNormal << std::endl;

		}
		if (specularMap != NULL)
		{
			size_t pixelIndex = (size_t) (specularMap->width * (specularMap->height - 1)
					- ((int) (texCoordinate.y * specularMap->height - 0.00001)) * specularMap->width +
					((int) (texCoordinate.x * specularMap->width - 0.00001)))* 4;
			intPoint.glossyRoughness = 1.0 - specularMap->pixels[pixelIndex] * 1.0 / 255.0;
			if (intPoint.glossyRoughness >= 0.999)
				intPoint.reflType = DIFF;
			else
				intPoint.reflType = GLOSS;
		}
	}
	return true;
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
