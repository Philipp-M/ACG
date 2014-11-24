#ifndef GTEXTUREDTRIANGLE_HPP_
#define GTEXTUREDTRIANGLE_HPP_

#include "DataTypes.hpp"
#include "GObject.hpp"
#include "TextureManager.hpp"
#include "MaterialManager.hpp"

class GTexturedTriangle : public GObject
{
public:

	GTexturedTriangle(Vec3 v0_, Vec3 v1_, Vec3 v2_, Vec2 u, Vec2 v, Vec2 w, const Material* mat) :
			v0(v0_), v1(v1_), v2(v2_), ut(u), vt(v), wt(w), mat(mat)
	{
		updateGeometry();
	}
	GTexturedTriangle(const GTexturedTriangle& other);

	Vec3 getNorm() const;
	Vec3 getColor() const;
	Vec3 getEmission() const;
	Refl_t getReflectionType() const;

	/***** implementations of GObject *****/
	/**
	 * for simplicity the triangle is also a "normal" Object
	 */
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

	//simple translation operations
	void translate(const Vec3& t);
	void rotationX(float rad);
	void rotationY(float rad);
	void rotationZ(float rad);

	//very simple liner acceleration
	void translateAcc(Vec3 t, double acc, long time);

	void rotation(float rad, const Vec3& dir);

	//for GPolyObject
	void rotationCentroid(Vec3 centroid,float rad, const Vec3& dir);
	virtual ~GTexturedTriangle() {}
private:
	/***** methods *****/
	/**
	 * updates the Geometry (which is  for now just the normal)
	 * the front face is determined like in open GL: Counter Clockwise: v0, v1, v2
	 */
	void updateGeometry() {	normal = ((v1 - v0)%(v2 - v0)).norm(); }
	/***** attributes *****/
	Vec3 v0;
	Vec3 v1;
	Vec3 v2;
	Vec2 ut;
	Vec2 vt;
	Vec2 wt;
	Vec3 normal;
	const Material* mat;
};

inline GTexturedTriangle::GTexturedTriangle(const GTexturedTriangle& other)
{
	v0 = other.v0;
	v1 = other.v1;
	v2 = other.v2;
	ut = other.ut;
	vt = other.vt;
	wt = other.wt;
	normal = other.normal;
	mat = other.mat;
}
#endif /* GTEXTUREDTRIANGLE_HPP_ */
