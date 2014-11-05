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
	for (int i = int(objects.size() - 1); i >= 0; i--)
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

