#include "GTexturedTriangle.hpp"
#include <cstdio>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

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

void GTexturedTriangle::translate(const Vec3& t) {
	v0 = v0 + t;
	v1 = v1 + t;
	v2 = v2 + t;
}

void GTexturedTriangle::rotationX(float rad) {
	rotation(rad, Vec3(1, 0, 0));
}
void GTexturedTriangle::rotationY(float rad) {
	rotation(rad, Vec3(0, 1, 0));
}
void GTexturedTriangle::rotationZ(float rad) {
	rotation(rad, Vec3(0, 0, 1));
}

void GTexturedTriangle::rotation(float rad, const Vec3& dir) {
	rotationCentroid(getCentroid(), rad, dir);
}

void GTexturedTriangle::rotationCentroid(Vec3 centroid, float rad, const Vec3& dir)
{
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

	v0 = Vec3(vec0.x, vec0.y, vec0.z);
	v1 = Vec3(vec1.x, vec1.y, vec1.z);
	v2 = Vec3(vec2.x, vec2.y, vec2.z);
	updateGeometry();
}

void GTexturedTriangle::translateAcc(Vec3 t, double acc, long time)
{
	Vec3 v = Vec3(t.x*acc*time, t.y*acc*time, t.z*acc*time);

	v0 = v0 + v;
	v1 = v1 + v;
	v2 = v2 + v;
	//updateGeometry() not necessary, since the normal does not change...
}
