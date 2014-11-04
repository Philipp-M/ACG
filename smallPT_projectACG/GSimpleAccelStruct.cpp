#include "GSimpleAccelStruct.hpp"
#include <cstdlib>
#include <limits>

const GPrimitiveObject* GSimpleAccelStruct::intersect(const Ray& ray, double& t) const
{
	t = std::numeric_limits<double>::infinity(); // c++ way to indicate infinity
	double dis;
	const GPrimitiveObject* retVal = NULL;
	const GPrimitiveObject* tmpObj = NULL;
	for (int i = int(objects.size() - 1); i >= 0; i--)
	{
		if(bboxes[i].intersect(ray))
		{
			if (((tmpObj = objects[i]->intersect(ray, dis)) != NULL) && dis < t)
			{
				t = dis;
				retVal = tmpObj;
			}
		}
	}
	return t < std::numeric_limits<double>::infinity() ? retVal : NULL;
}

