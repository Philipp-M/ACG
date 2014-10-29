#include "ObjLoader.hpp"
#include "DataTypes.hpp"
#include "tiny_obj_loader.hpp"
#include "GTriangle.hpp"
#include <iostream>
std::vector<GPolygonObject*> ObjLoader::loadOfFile(const char* filename, const char* MatPath)
{
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string err = tinyobj::LoadObj(shapes, materials, filename, MatPath);
	std::vector<GTriangle> faces;
	std::vector<GPolygonObject*> retObjs;
	if (!err.empty())
	{
		std::cerr << err << std::endl;
		exit(1);
	}

	for (size_t i = 0; i < shapes.size(); i++)
	{
		faces.clear();
		for (size_t j = 0; j < shapes[i].mesh.indices.size() / 3; j++)
		{
			Vec v0 = Vec(shapes[i].mesh.positions[shapes[i].mesh.indices[j * 3 + 0] * 3 + 0],
					shapes[i].mesh.positions[shapes[i].mesh.indices[j * 3 + 0] * 3 + 1], shapes[i].mesh.positions[shapes[i].mesh.indices[j * 3 + 0] * 3 + 2]);
			Vec v1 = Vec(shapes[i].mesh.positions[shapes[i].mesh.indices[j * 3 + 1] * 3 + 0],
					shapes[i].mesh.positions[shapes[i].mesh.indices[j * 3 + 1] * 3 + 1], shapes[i].mesh.positions[shapes[i].mesh.indices[j * 3 + 1] * 3 + 2]);
			Vec v2 = Vec(shapes[i].mesh.positions[shapes[i].mesh.indices[j * 3 + 2] * 3 + 0],
					shapes[i].mesh.positions[shapes[i].mesh.indices[j * 3 + 2] * 3 + 1], shapes[i].mesh.positions[shapes[i].mesh.indices[j * 3 + 2] * 3 + 2]);
			faces.push_back(GTriangle(v0, v1, v2, Vec(), Vec(1, 1, 1) * 0.999, DIFF)); // for now just a white diffuse type for testing purpose
		}
		retObjs.push_back(new GPolygonObject(faces));
	}
	return retObjs;
}
