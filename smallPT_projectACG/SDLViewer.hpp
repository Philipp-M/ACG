#ifndef SDLVIEWER_HPP_
#define SDLVIEWER_HPP_

#include <SDL2/SDL.h>
#include "DataTypes.hpp"

class SDLViewer
{
public:
	void display();
	SDLViewer(int w, int h, int sampleStep = 4);
	virtual ~SDLViewer();
private:
	void initSDL(int w, int h);
	static int renderThreadF(void* data);
	void saveImage();
	void handleEvents(SDL_Event& events, bool& needsUpdate);
	SDL_Surface* toneMap(Uint32* pixels);

	SDL_Window* window;
	SDL_Thread *renderThread;
	SDL_mutex* mutex;
	SDL_Surface* gScreenSurface;
	SDL_Surface* gButtonCurrent;
	SDL_Surface* gButtonDefault;
	SDL_Surface* gButtonPressed;
	SDL_Surface* gButtonHover;
	Vec* rawSamplesData;
	int curSPP;
	int sampleStep;
	bool imageNeedsRefresh;
	bool quit;

};

#endif /* SDLVIEWER_HPP_ */
