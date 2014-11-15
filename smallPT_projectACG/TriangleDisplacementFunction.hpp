#ifndef TRIANGLEDISPLACEMENTFUNCTION_HPP
#define TRIANGLEDISPLACEMENTFUNCTION_HPP
#include "DataTypes.hpp"
/**
 * interface for the displacement function, which can be e.g. just a displacement texture
 * or even a complex procedural texture generator(like combinations of e.g. perlin noise voronoi noise etc.)
 */
class TriangleDisplacementFunction
{
public:
	/**
	 * the Ray Intersection Point structure will only be used for color and material information(if at all),
	 * its kind of a bonus to the displacement function, that is just to fast implemented, to ignore it
	 * @param u texture coordinate has to fulfill the following restrictions: 0 <= u <= 1.0 && u+v <=1
	 * @param v texture coordinate has to fulfill the following restrictions: 0 <= v <= 1.0 && u+v <=1
	 * @param point the resulting calculated point, based on u and v
	 * @param intPoint used for material description, only color, emission and reflType are used
	 */
	virtual void getPoint(const double& u, const  double& v, Vec3& point, RayIntPt& intPoint) const = 0;

	virtual ~TriangleDisplacementFunction() {};
};

class DisplacementMapFunction : public TriangleDisplacementFunction
{
	//DisplacementMapFunction(const Vec2& u, const Vec2& v, const Vec2& w, int texID, double maxHeight);
};

#endif // TRIANGLEDISPLACEMENTFUNCTION_HPP
