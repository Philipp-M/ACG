#ifndef GDISPLACEDTRIANGLE_HPP_
#define GDISPLACEDTRIANGLE_HPP_

#include "GObject.hpp"
#include "TriangleDisplacementFunction.hpp"

class GDisplacedTriangle: public GObject
{
public:
	GDisplacedTriangle() { }
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
	Vec3 v0;
	Vec3 v1;
	Vec3 v2;
	Vec3 normal;
};

#endif /* GDISPLACEDTRIANGLE_HPP_ */
