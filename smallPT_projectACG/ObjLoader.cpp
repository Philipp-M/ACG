#include "ObjLoader.hpp"
#include "DataTypes.hpp"
#include "tiny_obj_loader.hpp"
#include "GTriangle.hpp"
#include "GTexturedTriangle.hpp"
#include "TextureManager.hpp"
#include "MaterialManager.hpp"
#include <iostream>
std::vector<GPolygonObject<GTexturedTriangle>*> ObjLoader::loadOfFile(const char* filename, const char* MatPath)
{
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::vector<uint16_t> texIDs;
	std::string err = tinyobj::LoadObj(shapes, materials, filename, MatPath);
	std::vector<GTexturedTriangle> faces;
	std::vector<GPolygonObject<GTexturedTriangle>*> retObjs;

	if (!err.empty())
	{
		std::cerr << err << std::endl;
		exit(1);
	}
	MaterialManager& mm = MaterialManager::get();
	TextureManager& tm = TextureManager::get();
	// parse all materials and add them to the material manager
	mm.addMaterial(Material("default", Vec3(1,1,1)*0.999, 1.0, Vec3(), NULL, NULL, NULL, DIFF));
	for (const tinyobj::material_t& mat : materials)
	{
		// load textures if they exist
		uint16_t ct = 0xFFFF, nt = 0xFFFF, st = 0xFFFF;
		if (mat.diffuse_texname.size() > 0)
			ct = tm.loadTexture(mat.diffuse_texname);
		texIDs.push_back(ct);
		if (mat.normal_texname.size() > 0)
			nt = tm.loadTexture(mat.normal_texname);
		texIDs.push_back(nt);
		if (mat.specular_texname.size() > 0)
			st = tm.loadTexture(mat.specular_texname);
		texIDs.push_back(st);

		// determine material type
		Refl_t refl = DIFF;
		Vec3 diff = Vec3(mat.diffuse)*0.999; // comolete white could end in endless paths
		Vec3 spec = Vec3(mat.specular)*0.999;
		Vec3 emis = Vec3(mat.emission);
		double massDiff = (diff.x+diff.y+diff.z)/3.0;
		double massSpec = (spec.x+spec.y+spec.z)/3.0;
		double massEmis = (emis.x+emis.y+emis.z)/3.0;
		double glossyFact = 1.0 - std::min(std::max(0.0, massSpec), 1.0);
		if (massEmis > 0)
			refl = EMIS;
		else if(mat.dissolve < 1.0)
			refl = REFR;
		else if(massDiff == 0 && massSpec > 0)
		{
			diff = spec;
			refl = SPEC;
		}
		else if(massDiff > 0 && massSpec > 0)
		{
			diff = Vec3(std::min(diff.x+spec.x, 1.0), std::min(diff.y+spec.y, 1.0), std::min(diff.z+spec.z, 1.0));
			refl = GLOSS;
		}

		mm.addMaterial(Material(mat.name, diff, glossyFact, emis, tm.getByID(ct), tm.getByID(nt), tm.getByID(st), refl));
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
				const Material* mat = mm.getByID(shapes[i].mesh.material_ids[j]+1);

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
					faces.push_back(GTexturedTriangle(v0, v1, v2, u, v, w, mat));
			}
			else
				faces.push_back(GTexturedTriangle(v0, v1, v2, Vec2(),Vec2(),Vec2(), mm.getByID(0))); // for default a white diffuse material
		}
		retObjs.push_back(new GPolygonObject<GTexturedTriangle>(faces));
	}
	return retObjs;
}
