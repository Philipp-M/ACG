#ifndef GTRIANGLE_HPP
#define GTRIANGLE_HPP

#include "DataTypes.hpp"
#include "GObject.hpp"
#include "GPrimitiveObject.hpp"

class GTriangle: public GPrimitiveObject, public GObject
{
public:

	GTriangle(Vec v0_, Vec v1_, Vec v2_, Vec emission_, Vec color_,  Refl_t refl_) :
			v0(v0_), v1(v1_), v2(v2_), emission(emission_), color(color_), refl(refl_)
	{
		updateGeometry();
	}
	/***** implementations of GPrimitiveObject *****/
	/*
	 * implemented intersection method of a triangle
	 */
	double intersect(const Ray &ray) const;

	Vec getNorm(const Vec& position) const;
	Vec getColor(const Vec& position) const;
	Vec getEmission(const Vec& position) const;
	Refl_t getReflectionType() const;

	/***** implementations of GObject *****/
	/**
	 * for simplicity the triangle is also a "normal" Object
	 */
	bool intersect(const Ray &ray, RayIntPt& intPoint) const;
	Vec getCentroid() const;

	/***** custom methods *****/
	void setV0(const Vec& v) { v0 = v; updateGeometry(); }
	void setV1(const Vec& v) { v1 = v; updateGeometry(); }
	void setV2(const Vec& v) { v2 = v; updateGeometry(); }
	void setV(const Vec& v0_, const Vec& v1_, const Vec& v2_) { v0 = v0_; v1 = v1_; v2 = v2_; updateGeometry(); }
	Vec getV0() const { return v0; }
	Vec getV1() const { return v1; }
	Vec getV2() const { return v2; }

	virtual ~GTriangle();
private:
	/***** methods *****/
	/**
	 * updates the Geometry (which is  for now just the normal)
	 * the front face is determined like in open GL: Counter Clockwise: v0, v1, v2
	 */
	void updateGeometry() {	normal = ((v1 - v0)%(v2 - v0)).norm(); }
	/***** attributes *****/
	Vec v0;
	Vec v1;
	Vec v2;
	Vec emission;
	Vec color;
	Vec normal;
	Refl_t refl;
};

#endif // GTRIANGLE_HPP
