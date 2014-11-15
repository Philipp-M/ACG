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
	Vec getCentroid() const;
	GBoundingBox createBoundingBox() const;

	/***** custom methods *****/
	void setV0(const Vec& v) { v0 = v; updateGeometry(); }
	void setV1(const Vec& v) { v1 = v; updateGeometry(); }
	void setV2(const Vec& v) { v2 = v; updateGeometry(); }
	void setV(const Vec& v0_, const Vec& v1_, const Vec& v2_) { v0 = v0_; v1 = v1_; v2 = v2_; updateGeometry(); }
	Vec getV0() const { return v0; }
	Vec getV1() const { return v1; }
	Vec getV2() const { return v2; }
private:
	void updateGeometry() {	normal = ((v1 - v0)%(v2 - v0)).norm(); }
	Vec v0;
	Vec v1;
	Vec v2;
	Vec normal;
};

#endif /* GDISPLACEDTRIANGLE_HPP_ */
