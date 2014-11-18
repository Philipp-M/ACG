#ifndef OBJLOADER_HPP_
#define OBJLOADER_HPP_

#include <vector>
#include "GPolygonObject.hpp"
#include "GTexturedTriangle.hpp"
class ObjLoader
{
public:
	static std::vector<GPolygonObject<GTexturedTriangle>*> loadOfFile(const char* filename, const char* MatPath = NULL);
};

#endif /* OBJLOADER_HPP_ */
