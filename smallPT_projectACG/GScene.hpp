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
	 * returns false if nothing was hit, if it intersects true will be returned and the intersection data will be put in
	 * intPoint
	 */
	bool intersect(const Ray &ray, RayIntPt& intPoint) const
	{
		return accelStruct->intersect(ray, intPoint);
	}
	/**
	 * just adds the pointer of the object to the scene,
	 * the object itself will be deleted in the destructor! so it should be 'private' for the scene...
	 */
	void addItem(GObject* object, bool update = true);
private:
	GAccelStruct* accelStruct;
	std::vector<GObject*> objects;
};

#endif /* GEOMETRICSCENE_HPP_ */
