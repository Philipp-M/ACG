#include "TextureManager.hpp"
#include <SDL2/SDL.h>
#include <iostream>
#include <cstring>

int TextureManager::loadTexture(const std::string& filename)
{
	SDL_Surface* tmpSurface = SDL_LoadBMP(filename.c_str());
	if (!tmpSurface)
	{
		std::cerr << "Error(0) while loading texture: " << filename << std::endl;
		exit(-1);
	}
	SDL_Surface* loadedSurface = SDL_ConvertSurfaceFormat(tmpSurface, SDL_PIXELFORMAT_ABGR8888, 0);
	if (!loadedSurface)
	{
		std::cerr << "Error(0) while loading texture: " << filename << std::endl;
		exit(-1);
	}
	Texture tex;
	tex.name = filename;
	tex.width = loadedSurface->w;
	tex.height = loadedSurface->h;
	tex.pixels = new uint8_t[tex.width * tex.height*4];

	memcpy(tex.pixels, loadedSurface->pixels, sizeof(uint8_t) * 4 * tex.width * tex.height);
	SDL_FreeSurface(tmpSurface);
	SDL_FreeSurface(loadedSurface);
	textures.push_back(tex);
	return textures.size()-1;
}
