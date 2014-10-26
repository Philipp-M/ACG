#ifndef GSIMPLEACCELSTRUCT_HPP
#define GSIMPLEACCELSTRUCT_HPP

#include "GAccelStruct.hpp"
#include "GPrimitiveObject.hpp"
#include "GObject.hpp"
#include <vector>

class GSimpleAccelStruct : public GAccelStruct
{
public:
	GSimpleAccelStruct(const std::vector<GObject*>& objects_) : objects(objects_) {}
	const GPrimitiveObject* intersect(const Ray& ray, double& t) const;
	virtual ~GSimpleAccelStruct() {}
private:
	const std::vector<GObject*>& objects;
};

#endif // GSIMPLEACCELSTRUCT_HPP
