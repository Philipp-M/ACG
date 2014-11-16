#ifndef TRIANGLEDISPLACEMENTFUNCTION_HPP
#define TRIANGLEDISPLACEMENTFUNCTION_HPP
#include "DataTypes.hpp"
#include "TextureManager.hpp"
#include <cstdint>
#include <iostream>
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
	 * @param point the resulting calculated point, based on u and v and the previous point
	 * @param intPoint used for material description, only color, emission and reflType are used
	 */
	virtual double getPoint(double u, double v, RayIntPt& intPoint) const = 0;

	virtual ~TriangleDisplacementFunction() {};
};

class DisplacementMapFunction: public TriangleDisplacementFunction
{
public:
	DisplacementMapFunction(const Vec2& u, const Vec2& v, const Vec2& w, uint32_t texID, double maxHeight);
	double getPoint(double u, double v, RayIntPt& intPoint) const;
private:
	double maxHeight;
	uint32_t texID;
	Vec2 u, v, w;
	const Texture* dMap;
};

DisplacementMapFunction::DisplacementMapFunction(const Vec2& u, const Vec2& v, const Vec2& w, uint32_t texID, double maxHeight) :
		maxHeight(maxHeight), texID(texID), u(u), v(v), w(w), dMap(TextureManager::get().getByID(texID))
{
	if(dMap == NULL)
	{
		std::cerr<<"Error while getting the texture map with id: " << texID << std::endl;
		exit(0);
	}
}

double DisplacementMapFunction::getPoint(double u, double v, RayIntPt& intPoint) const
{
	Vec2 texCoordinate = (this->w + (this->u - this->w) * u + (this->v - this->w) * v).mult(Vec2(dMap->width, dMap->height));
	// takes the red channel as displacement, since they should all be the same...
	return dMap->pixels[(uint32_t)(texCoordinate.y*dMap->width + texCoordinate.x)*4]*1.0/256.0*maxHeight;
}

#endif // TRIANGLEDISPLACEMENTFUNCTION_HPP
