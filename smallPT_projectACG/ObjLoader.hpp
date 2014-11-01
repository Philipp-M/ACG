#ifndef OBJLOADER_HPP_
#define OBJLOADER_HPP_

#include <vector>
#include "GPolygonObject.hpp"

class ObjLoader
{
public:
	static std::vector<GPolygonObject*> loadOfFile(const char* filename, const char* MatPath = NULL);
};

#endif /* OBJLOADER_HPP_ */
