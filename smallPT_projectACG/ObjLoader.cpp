#include "ObjLoader.hpp"
#include "DataTypes.hpp"
#include "tiny_obj_loader.hpp"
#include "GTriangle.hpp"
#include <iostream>
std::vector<GPolygonObject<GTriangle>*> ObjLoader::loadOfFile(const char* filename, const char* MatPath)
{
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string err = tinyobj::LoadObj(shapes, materials, filename, MatPath);
	std::vector<GTriangle> faces;
	std::vector<GPolygonObject<GTriangle>*> retObjs;
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

			if(materials.size() > 0)
			{
				/* for now just a very basic material support, because smallPT has not the best material support
				 * can change maybe in future versions (e.g when displacement mapping will be implemented
				 * Simple hack for lightning, (because blender doesn't support the ambient lighting in an easy way):
				 * if diffuse lightning and specular color(sum of the 3 components > 1) is defined,
				 * the diffuse color will be taken as light source
				 */
				const tinyobj::material_t& mat = materials[shapes[i].mesh.material_ids[j]];
				if(mat.dissolve < 1)
					faces.push_back(GTriangle(v0, v1, v2, Vec(), Vec(mat.diffuse[0], mat.diffuse[1], mat.diffuse[2]) * 0.999, REFR));
				else if(mat.diffuse[0] + mat.diffuse[1] + mat.diffuse[2] > 0.01 && mat.specular[0] + mat.specular[1] + mat.specular[2] > 1)
					faces.push_back(GTriangle(v0, v1, v2, Vec(mat.diffuse[0], mat.diffuse[1], mat.diffuse[2]), Vec(), DIFF));
				else if(mat.specular[0] + mat.specular[1] + mat.specular[2] > 1)
					faces.push_back(GTriangle(v0, v1, v2, Vec(), Vec(mat.specular[0], mat.specular[1], mat.specular[2]) * 0.999, SPEC));
				else
					faces.push_back(GTriangle(v0, v1, v2, Vec(), Vec(mat.diffuse[0], mat.diffuse[1], mat.diffuse[2]) * 0.999, DIFF));
			}
			else
				faces.push_back(GTriangle(v0, v1, v2, Vec(), Vec(1, 1, 1) * 0.999, DIFF)); // for default a white diffuse material
		}
		retObjs.push_back(new GPolygonObject<GTriangle>(faces));
	}
	return retObjs;
}
