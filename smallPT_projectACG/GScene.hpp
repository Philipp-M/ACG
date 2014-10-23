#ifndef GEOMETRICSCENE_HPP_
#define GEOMETRICSCENE_HPP_

#include <vector>
#include <iostream>
#include "DataTypes.hpp"
#include "GPrimitiveObject.hpp"
#include "GObject.hpp"
#include <limits>

// Ray intersection Point, contains information about the intersection

class GScene
{
public:
	GScene();
	~GScene();
	/**
	 * returns NULL if nothing was hit, else the pointer to the corresponding intersection Object
	 *
	 * definitely needs optimization
	 */
	const GPrimitiveObject * intersect(const Ray &ray, double& t) const
	{
		t = 1e20;//std::numeric_limits<double>::infinity(); // c++ way to indicate infinity
		double dis;
		const GPrimitiveObject* retVal = NULL;
		const GPrimitiveObject* tmpObj = NULL;
		for (int i = int(objects.size()-1); i >= 0; i--)
		{
			if (((tmpObj = objects[i]->intersect(ray, dis)) != NULL) && dis < t)
			{
				t = dis;
				retVal = tmpObj;
			}
		}
		return t < 1e20 ? retVal : NULL;
	}
	void addItem(GObject* object);
private:
	std::vector<GObject*> objects;
	// should contain an acceleration structure in future (obviously a BVH)
};

#endif /* GEOMETRICSCENE_HPP_ */
