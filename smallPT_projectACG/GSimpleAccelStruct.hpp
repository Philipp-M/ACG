#ifndef GSIMPLEACCELSTRUCT_HPP
#define GSIMPLEACCELSTRUCT_HPP

#include "GAccelStruct.hpp"
#include "GBVHAccelStruct.hpp"
#include "GPrimitiveObject.hpp"
#include "GObject.hpp"
#include <vector>

class GSimpleAccelStruct : public GAccelStruct
{
public:
	GSimpleAccelStruct(const std::vector<GObject*>& objects_) : objects(objects_)
	{
		for(const GObject* o : objects)
			bboxes.push_back(o->createBoundingBox());
	}
	bool intersect(const Ray &ray, RayIntPt& intPoint) const;
	virtual ~GSimpleAccelStruct() {}
private:
	std::vector<GBoundingBox> bboxes;
	const std::vector<GObject*>& objects;
};

#endif // GSIMPLEACCELSTRUCT_HPP
