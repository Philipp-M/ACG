#ifndef TEXTUREMANAGER_HPP_
#define TEXTUREMANAGER_HPP_
#include <vector>
#include <string>
#include <cstdint>

/**
 * simple texture structure, containing pixeldata and the size of the image
 */
struct Texture
{
	std::string name; // maybe not needed
	size_t width;
	size_t height;
	uint8_t *pixels;
};
/**
 * very simple texture manager singleton, its quite easy to understand...
 */
class TextureManager
{
public:
	/**
	 * if the texture manager gets destroyed, it also deletes all the textures it holds...
	 */
	virtual ~TextureManager()
	{
		for( const Texture& tex : textures)
			delete[] tex.pixels;
	}
	/*** Singleton Instance ***/
	static TextureManager& get()
	{
		static TextureManager instance;
		return instance;
	}
	/**
	 * for now just 'BMP' support
	 * loads a texture with the SDL functions
	 * returns the id of the texture...
	 */
	int loadTexture(const std::string& filename);
	/**
	 * get texture pointer(which is const for a reason ;) ) by id
	 */
	const Texture* getByID (uint16_t id) { return textures.size() <= id ? NULL : &textures[id]; }
private:
	std::vector<Texture> textures;
	// private constructor to prevent instancing
	TextureManager() { }
};

#endif /* TEXTUREMANAGER_HPP_ */
