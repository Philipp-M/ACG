#ifndef GTRIANGLE_H_
#define GTRIANGLE_H_

#include "DataTypes.hpp"
#include "GObject.hpp"
#include "GPrimitiveObject.hpp"

class GTriangle: public GPrimitiveObject, public GObject
{
public:

	GTriangle(Vec v0_, Vec v1_, Vec v2_, Vec emission_, Vec color_,  Refl_t refl_) :
			v0(v0_), v1(v1_), v2(v2_), emission(emission_), color(color_), refl(refl_)
	{
		Vec edge1 = v1 - v0;
		Vec edge2 = v2 - v0;
		normal = (edge1%edge2).norm();
	}
	double intersect(const Ray &ray) const;

	Vec getNorm(const Vec& position) const;
	Vec getColor(const Vec& position) const;
	Vec getEmission(const Vec& position) const;

	const GPrimitiveObject* intersect(const Ray& ray, double& t) const;

	Refl_t getReflectionType() const;
	virtual ~GTriangle();
private:
	Vec v0;
	Vec v1;
	Vec v2;
	Vec emission;
	Vec color;
	Vec normal;
	Refl_t refl;
};

#endif /* GTRIANGLE_H_ */
