#ifndef GEOMETRICSCENE_HPP_
#define GEOMETRICSCENE_HPP_

#include <vector>
#include <iostream>
#include "DataTypes.hpp"
#include "GPrimitiveObject.hpp"
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
	const GPrimitiveObject * intersect(const Ray &ray, double& t) const
	{
		return accelStruct->intersect(ray, t);
	}
	void addItem(GObject* object, bool update = true);
private:
	GAccelStruct* accelStruct;
	std::vector<GObject*> objects;
};

#endif /* GEOMETRICSCENE_HPP_ */
