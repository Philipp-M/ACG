#ifndef TEXTUREMANAGER_HPP_
#define TEXTUREMANAGER_HPP_
#include <vector>
#include <string>
#include <cstdint>
struct Texture
{
	std::string name; // maybe not needed
	size_t width;
	size_t height;
	uint8_t *pixels;
};

class TextureManager
{
public:
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
	 */
	int loadTexture(const std::string& filename);
	const Texture* getByID (uint16_t id) { return textures.size() <= id ? NULL : &textures[id]; }
private:
	std::vector<Texture> textures;
	// private constructor to prevent instancing
	TextureManager() { }
};

#endif /* TEXTUREMANAGER_HPP_ */
