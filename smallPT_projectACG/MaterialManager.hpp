#ifndef MATERIALMANAGER_HPP_
#define MATERIALMANAGER_HPP_
#include "DataTypes.hpp"
#include "TextureManager.hpp"
#include <vector>
#include <string>

struct Material
{
	Material(const std::string& name, const Vec3& color, double glossyFact, const Vec3& emission, const Texture* cMap, const Texture* nMap, const Texture* sMap, Refl_t refl) :
			name(name), color(color), glossyFact(glossyFact), emission(emission), colorMap(cMap), normalMap(nMap), specularMap(sMap), refl(refl) { }
	std::string name;
	Vec3 color;
	double glossyFact;
	Vec3 emission;
	const Texture* colorMap;
	const Texture* normalMap;
	const Texture* specularMap;
	Refl_t refl;
};

class MaterialManager
{
public:
	static MaterialManager& get()
	{
		static MaterialManager instance;
		return instance;
	}
	uint16_t addMaterial(const Material& material) { materials.push_back(material); return materials.size()-1; }
	const Material* getByID (uint16_t id) { return materials.size() <= id ? NULL : &materials[id]; }
private:
	std::vector<Material> materials;
	MaterialManager() { }
};

#endif /* MATERIALMANAGER_HPP_ */
