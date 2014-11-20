#include "ObjLoader.hpp"
#include "DataTypes.hpp"
#include "tiny_obj_loader.hpp"
#include "GTriangle.hpp"
#include "GTexturedTriangle.hpp"
#include "TextureManager.hpp"
#include <iostream>
std::vector<GPolygonObject<GTexturedTriangle>*> ObjLoader::loadOfFile(const char* filename, const char* MatPath)
{
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::vector<uint16_t> texIDs;
	std::string err = tinyobj::LoadObj(shapes, materials, filename, MatPath);
	std::vector<GTexturedTriangle> faces;
	std::vector<GPolygonObject<GTexturedTriangle>*> retObjs;

	TextureManager& tm = TextureManager::get();

	if (!err.empty())
	{
		std::cerr << err << std::endl;
		exit(1);
	}
	/*
	 * load all textures from the material library
	 */
	for (const tinyobj::material_t& mat : materials)
	{
		uint16_t tmp = 0xFFFF;
		if (mat.diffuse_texname.size() > 0)
			tmp = tm.loadTexture(mat.diffuse_texname);
		texIDs.push_back(tmp);
		tmp = 0xFFFF;
		if (mat.normal_texname.size() > 0)
			tmp = tm.loadTexture(mat.normal_texname);
		texIDs.push_back(tmp);
		tmp = 0xFFFF;
		if (mat.specular_texname.size() > 0)
			tmp = tm.loadTexture(mat.specular_texname);
		texIDs.push_back(tmp);
	}
	for (size_t i = 0; i < shapes.size(); i++)
	{
		faces.clear();
		for (size_t j = 0; j < shapes[i].mesh.indices.size() / 3; j++)
		{
			Vec3 v0 = Vec3(shapes[i].mesh.positions[shapes[i].mesh.indices[j * 3 + 0] * 3 + 0],
					shapes[i].mesh.positions[shapes[i].mesh.indices[j * 3 + 0] * 3 + 1], shapes[i].mesh.positions[shapes[i].mesh.indices[j * 3 + 0] * 3 + 2]);
			Vec3 v1 = Vec3(shapes[i].mesh.positions[shapes[i].mesh.indices[j * 3 + 1] * 3 + 0],
					shapes[i].mesh.positions[shapes[i].mesh.indices[j * 3 + 1] * 3 + 1], shapes[i].mesh.positions[shapes[i].mesh.indices[j * 3 + 1] * 3 + 2]);
			Vec3 v2 = Vec3(shapes[i].mesh.positions[shapes[i].mesh.indices[j * 3 + 2] * 3 + 0],
					shapes[i].mesh.positions[shapes[i].mesh.indices[j * 3 + 2] * 3 + 1], shapes[i].mesh.positions[shapes[i].mesh.indices[j * 3 + 2] * 3 + 2]);

			if(materials.size() > 0)
			{
				/* for now just a very basic material support, because smallPT has not the best material support
				 * can change maybe in future versions
				 * Simple hack for lightning, (because blender doesn't support the ambient lighting in an easy way):
				 * if diffuse lightning and specular color(sum of the 3 components > 1) is defined,
				 * the diffuse color will be taken as light source
				 * if alpha(d) is smaller than 1, the material is treated as refractive
				 * if one(two or all three) of the 3 textures is defined,
				 * the texture will be loaded and added to the textured triangle (and of course the texture manager)
				 */
				const tinyobj::material_t& mat = materials[shapes[i].mesh.material_ids[j]];
				const Texture* colorMap = NULL;
				const Texture* normalMap = NULL;
				const Texture* specularMap = NULL;
				if (mat.diffuse_texname.size() > 0)
					colorMap = tm.getByID(texIDs[shapes[i].mesh.material_ids[j] * 3 + 0]);
				if (mat.normal_texname.size() > 0)
					normalMap = tm.getByID(texIDs[shapes[i].mesh.material_ids[j] * 3 + 1]);
				if (mat.specular_texname.size() > 0)
					specularMap = tm.getByID(texIDs[shapes[i].mesh.material_ids[j] * 3 + 2]);
				Vec2 u,v,w;
				if (shapes[i].mesh.texcoords.size() > shapes[i].mesh.indices[j * 3 + 2] * 2 + 1)
				{
					u = Vec2(fabs(shapes[i].mesh.texcoords[shapes[i].mesh.indices[j * 3 + 0] * 2 + 0])>1 ? 1.0 :fabs(shapes[i].mesh.texcoords[shapes[i].mesh.indices[j * 3 + 0] * 2 + 0]),
							fabs(shapes[i].mesh.texcoords[shapes[i].mesh.indices[j * 3 + 0] * 2 + 1])>1 ? 1.0 :fabs(shapes[i].mesh.texcoords[shapes[i].mesh.indices[j * 3 + 0] * 2 + 1]));
					v = Vec2(fabs(shapes[i].mesh.texcoords[shapes[i].mesh.indices[j * 3 + 1] * 2 + 0])>1 ? 1.0 :fabs(shapes[i].mesh.texcoords[shapes[i].mesh.indices[j * 3 + 1] * 2 + 0]),
							fabs(shapes[i].mesh.texcoords[shapes[i].mesh.indices[j * 3 + 1] * 2 + 1])>1 ? 1.0 :fabs(shapes[i].mesh.texcoords[shapes[i].mesh.indices[j * 3 + 1] * 2 + 1]));
					w = Vec2(fabs(shapes[i].mesh.texcoords[shapes[i].mesh.indices[j * 3 + 2] * 2 + 0])>1 ? 1.0 :fabs(shapes[i].mesh.texcoords[shapes[i].mesh.indices[j * 3 + 2] * 2 + 0]),
							fabs(shapes[i].mesh.texcoords[shapes[i].mesh.indices[j * 3 + 2] * 2 + 1])>1 ? 1.0 :fabs(shapes[i].mesh.texcoords[shapes[i].mesh.indices[j * 3 + 2] * 2 + 1]));

				}
				if(mat.dissolve < 1)
					faces.push_back(GTexturedTriangle(v0, v1, v2, u, v, w, colorMap, normalMap, specularMap, Vec3(), Vec3(mat.diffuse[0], mat.diffuse[1], mat.diffuse[2]) * 0.999, REFR));
				else if(mat.diffuse[0] + mat.diffuse[1] + mat.diffuse[2] > 0.01 && mat.specular[0] + mat.specular[1] + mat.specular[2] > 1)
					faces.push_back(GTexturedTriangle(v0, v1, v2, u, v, w, colorMap, normalMap, specularMap, Vec3(mat.diffuse[0], mat.diffuse[1], mat.diffuse[2]), Vec3(), DIFF));
				else if(mat.specular[0] + mat.specular[1] + mat.specular[2] > 1)
					faces.push_back(GTexturedTriangle(v0, v1, v2, u, v, w, colorMap, normalMap, specularMap, Vec3(), Vec3(mat.specular[0], mat.specular[1], mat.specular[2]) * 0.999, SPEC));
				else
					faces.push_back(GTexturedTriangle(v0, v1, v2, u, v, w, colorMap, normalMap, specularMap, Vec3(), Vec3(mat.diffuse[0], mat.diffuse[1], mat.diffuse[2]) * 0.999, DIFF));
			}
			else
				faces.push_back(GTexturedTriangle(v0, v1, v2, Vec2(),Vec2(),Vec2(),NULL,NULL,NULL, Vec3(), Vec3(1, 1, 1) * 0.999, DIFF)); // for default a white diffuse material
		}
		retObjs.push_back(new GPolygonObject<GTexturedTriangle>(faces));
	}
	return retObjs;
}
