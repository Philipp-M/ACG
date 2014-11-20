#include "GSimpleAccelStruct.hpp"
#include "DataTypes.hpp"
#include <cstdlib>
#include <limits>

bool GSimpleAccelStruct::intersect(const Ray &ray, RayIntPt& intPoint) const
{
	double dis, tmp;
	RayIntPt tmpIntPoint;
	intPoint.distance = std::numeric_limits<double>::infinity(); // c++ way to indicate infinity
	bool hit = false;

	/**
	 * following is mainly the approach, that smallPT chose,
	 * there is just one difference, because more complex Objects are introduced, a bounding box speeds up the
	 * intersection check a quite a bit
	 */
	for (size_t i = 0; i < objects.size(); i++)
	{
		if(bboxes[i].intersect(ray, dis, tmp))
		{
			if (objects[i]->intersect(ray, tmpIntPoint) && tmpIntPoint.distance < intPoint.distance)
			{
				intPoint = tmpIntPoint;
				hit = true;
			}
		}
	}
	return hit;
}

