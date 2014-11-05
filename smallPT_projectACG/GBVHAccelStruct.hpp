#ifndef GBVHACCELSTRUCT_HPP_
#define GBVHACCELSTRUCT_HPP_

#include "DataTypes.hpp"
#include "GObject.hpp"
#include <algorithm>
#include <vector>

class GBVHAccelStruct
{
public:
	GBVHAccelStruct(const std::vector<GObject*>& objects_);
	virtual ~GBVHAccelStruct();
private:
	GBoundingBox calculateBoundingBox(const std::vector<GBoundingBox>& bboxes);
};

#endif /* GBVHACCELSTRUCT_HPP_ */
