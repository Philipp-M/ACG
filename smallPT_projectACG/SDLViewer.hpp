#ifndef SDLVIEWER_HPP_
#define SDLVIEWER_HPP_

#include <SDL2/SDL.h>
#include "DataTypes.hpp"
#include "CameraModel.hpp"

class SDLViewer {
public:
	SDLViewer(int w, int h, std::string pathToScene = "scenes/cornell.obj",
			int sampleStep = 4, int timeSteps = 10);
	virtual ~SDLViewer();
	/**
	 * the main loop, waits for a refresh of the render Thread, and then updates the screen with the 'freshest' data
	 */
	void display();
private:
	/**
	 * inits all the stuff needed for SDL
	 */
	void initSDL(int w, int h);
	/**
	 * the main render Thread, it mostly contains the rest of smallPT's main loop
	 */
	static int renderThreadF(void* data);
	/**
	 * obviously just saves an image with the pattern renderxxxspp.bmp
	 */
	void saveImage();
	/**
	 * handles all input, mainly needed for the save picture button...
	 */
	void handleEvents(SDL_Event& events, bool& needsUpdate);
	/**
	 * a very simple function, that just takes the average of all sampled pixels, maps it to a SDL_Surface,
	 * for further operation on that, like saving or the update of the screen...
	 */
	SDL_Surface* toneMap(Uint32* pixels);

	SDL_Window* window;
	SDL_Thread *renderThread;
	SDL_mutex* mutex;
	SDL_Surface* gScreenSurface;
	SDL_Surface* gButtonCurrent;
	SDL_Surface* gButtonDefault;
	SDL_Surface* gButtonPressed;
	SDL_Surface* gButtonHover;
	Vec3* rawSamplesData;
	std::string pathToScene;
	int curSPP;
	int sampleStep;
	int timeSteps;
	bool imageNeedsRefresh;
	bool quit;
	long timeElapsed;

};

#endif /* SDLVIEWER_HPP_ */
