#ifndef OBJLOADER_HPP_
#define OBJLOADER_HPP_

#include <vector>
#include "GPolygonObject.hpp"
#include "GTriangle.hpp"
class ObjLoader
{
public:
	static std::vector<GPolygonObject<GTriangle>*> loadOfFile(const char* filename, const char* MatPath = NULL);
};

#endif /* OBJLOADER_HPP_ */
