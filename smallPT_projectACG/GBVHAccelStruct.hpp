#ifndef GBVHACCELSTRUCT_HPP_
#define GBVHACCELSTRUCT_HPP_

#include "DataTypes.hpp"
#include "GObject.hpp"
#include "GAccelStruct.hpp"
#include <algorithm>
#include <vector>

class Octree;

class GBVHAccelStruct : public GAccelStruct
{
public:
	GBVHAccelStruct(const std::vector<GObject*>& objects_);
	bool intersect(const Ray &ray, RayIntPt& intPoint) const;
	virtual ~GBVHAccelStruct();
private:
	GBoundingBox calculateBoundingBox(const std::vector<GBoundingBox>& bboxes);
	Octree* octree;
};

#endif /* GBVHACCELSTRUCT_HPP_ */
