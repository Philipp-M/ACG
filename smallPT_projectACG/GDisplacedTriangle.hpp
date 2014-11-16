#ifndef GDISPLACEDTRIANGLE_HPP_
#define GDISPLACEDTRIANGLE_HPP_

#include "GObject.hpp"
#include "TriangleDisplacementFunction.hpp"

class GDisplacedTriangle: public GObject
{
public:
	GDisplacedTriangle(TriangleDisplacementFunction* dispFunc, double maxHeight, uint16_t subdivisionAmount, Vec3 v0_, Vec3 v1_, Vec3 v2_, Vec3 emission_, Vec3 color_, Refl_t refl_) :
			maxHeight(maxHeight), subdivisionAmount(subdivisionAmount), v0(v0_), v1(v1_), v2(v2_), emission(emission_), color(color_), refl(refl_), dFunc(dispFunc)
	{
		updateGeometry();
	}
	GDisplacedTriangle(const GDisplacedTriangle& other)
	{
		maxHeight = other.maxHeight;
		v0 = other.v0;
		v1 = other.v1;
		v2 = other.v2;
		normal = other.normal;
		color = other.color;
		emission = other.emission;
		refl = other.refl;
	}
	virtual ~GDisplacedTriangle() { }

	/***** implementations of GObject *****/
	bool intersect(const Ray &ray, RayIntPt& intPoint) const;
	Vec3 getCentroid() const;
	GBoundingBox createBoundingBox() const;

	/***** custom methods *****/
	void setV0(const Vec3& v) { v0 = v; updateGeometry(); }
	void setV1(const Vec3& v) { v1 = v; updateGeometry(); }
	void setV2(const Vec3& v) { v2 = v; updateGeometry(); }
	void setV(const Vec3& v0_, const Vec3& v1_, const Vec3& v2_) { v0 = v0_; v1 = v1_; v2 = v2_; updateGeometry(); }
	Vec3 getV0() const { return v0; }
	Vec3 getV1() const { return v1; }
	Vec3 getV2() const { return v2; }
private:
	void updateGeometry() {	normal = ((v1 - v0)%(v2 - v0)).norm(); }
	double maxHeight;
	uint16_t subdivisionAmount;
	Vec3 v0;
	Vec3 v1;
	Vec3 v2;
	Vec3 normal;
	Vec3 emission;
	Vec3 color;
	Refl_t refl;

	TriangleDisplacementFunction* dFunc;
};

#endif /* GDISPLACEDTRIANGLE_HPP_ */
