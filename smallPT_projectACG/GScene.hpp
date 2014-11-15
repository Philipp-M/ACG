#ifndef GEOMETRICSCENE_HPP_
#define GEOMETRICSCENE_HPP_

#include <vector>
#include <iostream>
#include "DataTypes.hpp"
#include "GObject.hpp"
#include "GAccelStruct.hpp"

class GScene
{
public:
	GScene();
	~GScene();
	/**
	 * returns NULL if nothing was hit, else the pointer to the corresponding intersection Object
	 */
	bool intersect(const Ray &ray, RayIntPt& intPoint) const
	{
		return accelStruct->intersect(ray, intPoint);
	}
	void addItem(GObject* object, bool update = true);

	GAccelStruct* accelStruct;
	std::vector<GObject*> objects;
};

#endif /* GEOMETRICSCENE_HPP_ */
