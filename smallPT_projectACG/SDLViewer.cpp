#include "SDLViewer.hpp"
#include "SmallPT.hpp"
#include <iostream>
#include <vector>
#include <string>
#include "GScene.hpp"
#include "ObjLoader.hpp"
#include "GPolygonObject.hpp"
#include "CameraModel.hpp"

void SDLViewer::display()
{
	// Event handler
	SDL_Event events;
	while (!quit)
	{
		bool needsUpdate = false;
		if (imageNeedsRefresh)
			needsUpdate = true;
		// Handle events on queue
		handleEvents(events, needsUpdate);

		if (needsUpdate)
		{
			if (imageNeedsRefresh)
			{
				Uint32 *pixels = new Uint32[gScreenSurface->w * gScreenSurface->h];
				SDL_Surface* tonemappedImage = toneMap(pixels);
				if (tonemappedImage != NULL)
				{
					SDL_BlitSurface(tonemappedImage, NULL, gScreenSurface, NULL);
					SDL_FreeSurface(tonemappedImage);
				}
				delete[] pixels;
			}
			SDL_Rect buttonRect =
			{ gScreenSurface->w - gButtonDefault->w, gScreenSurface->h - gButtonDefault->h, gButtonDefault->w, gButtonDefault->h };
			SDL_BlitSurface(gButtonCurrent, NULL, gScreenSurface, &buttonRect);
			SDL_UpdateWindowSurface(window);
		}

		SDL_Delay(20); // wait a little bit
	}
	SDL_WaitThread(renderThread, NULL);
}

SDLViewer::SDLViewer(int w, int h, std::string pathToScene, int sampleStep) :
		pathToScene(pathToScene), curSPP(0), sampleStep(sampleStep), quit(false)
{
	if (this->sampleStep < 4)
		this->sampleStep = 4;
	initSDL(w, h);
}

SDLViewer::~SDLViewer()
{
	// Deallocate surface
	SDL_FreeSurface(gButtonDefault);
	SDL_FreeSurface(gButtonPressed);
	SDL_FreeSurface(gButtonHover);
	// Destroy window
	SDL_DestroyWindow(window);
	SDL_DestroyMutex(mutex);
	delete rawSamplesData;
	SDL_Quit();
}

void SDLViewer::initSDL(int w, int h)
{
	/******** init graphics ********/
	if (SDL_Init( SDL_INIT_VIDEO) < 0)
	{
		std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
		exit(-1);
	}
	// Create window
	window = SDL_CreateWindow("smallPT Project", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w, h, SDL_WINDOW_SHOWN);
	if (window == NULL)
	{
		std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
		exit(-1);
	}
	// Get window surface
	gScreenSurface = SDL_GetWindowSurface(window);
	// Load Button images
	gButtonDefault = SDL_LoadBMP("res/saveButton.bmp");
	gButtonCurrent = gButtonDefault;
	gButtonHover = SDL_LoadBMP("res/saveButtonH.bmp");
	gButtonPressed = SDL_LoadBMP("res/saveButtonP.bmp");

	if (gButtonDefault == NULL || gButtonHover == NULL || gButtonPressed == NULL)
	{
		std::cerr << "Button images couldn't be loaded! SDL_Error: " << SDL_GetError() << std::endl;
		exit(-1);
	}
	/******** init Renderer ********/
	rawSamplesData = new Vec[gScreenSurface->w * gScreenSurface->h];
	mutex = SDL_CreateMutex();
	renderThread = SDL_CreateThread(this->renderThreadF, "render Thread", this);
}

/**
 * following method is quite dirty, and should be cleaned up after implementing several
 * necessary features like the whole scene(not just the geometric model) or an extra camera class etc.
 * also kinda static...
 *
 * ugly SDL_thread workaround with "this" pointer was also necessary...
 */
int SDLViewer::renderThreadF(void* data)
{
	SDLViewer* viewer = reinterpret_cast<SDLViewer*>(data);
	int w = viewer->gScreenSurface->w;
	int h = viewer->gScreenSurface->h;
/*	Ray cam(Vec(50, 50, 230), Vec(0, 0, -1).norm()); // camera pos, dir
	Vec cx = Vec(w * .7135 / h); // x direction increment (uses implicit 0 for y, z)
	Vec cy = (cx % cam.direction).norm() * .7135; // y direction increment (note cross product)
*/
	CameraModel camera = CameraModel(Vec(50, 50, 230), Vec(0, 0, -1).norm(), 40.88, 150, 5, w, h);
	Vec r; // used for colors of samples
	int samps = viewer->sampleStep / 4;
	/********** create the scene *************/
	viewer->timeElapsed = SDL_GetTicks();
	GScene scene;
	std::vector<GPolygonObject*> sceneObj;
	sceneObj = ObjLoader::loadOfFile(viewer->pathToScene.c_str(), viewer->pathToScene.substr(0,viewer->pathToScene.find_last_of("/\\")+1).c_str());
	for (size_t i = 0; i < sceneObj.size(); i++)
		scene.addItem(sceneObj[i]);
	std::cerr << "time needed for building the scene: " <<(double)(SDL_GetTicks() - viewer->timeElapsed) / 1000.0 << " s\n\n"; // print progress
	viewer->timeElapsed = SDL_GetTicks();
	while (!viewer->quit)
	{
		unsigned long timeDeltaElapsed = SDL_GetTicks();
		Vec *c = new Vec[w * h]; // stack would be better, but can be exceeded, which causes a Segmention Fault...
		unsigned int seed = SDL_GetTicks();
#pragma omp parallel for schedule(dynamic, 1) private(r) // OpenMP
		for (unsigned short y = 0; y < h; y++)
		{ // Loop over image rows
			for (unsigned short x = 0, Xi[3] =
			{ 0, (unsigned short) (seed % (1 << 16)), (unsigned short) (y * y * y) }; x < w; x++)   // Loop columns
			{
				// FOR EACH PIXEL DO 2x2 SUBSAMPLES, AND samps SAMPLES PER SUBSAMPLE
				for (int sy = 0, i = (h - y - 1) * w + x; sy < 2; sy++)     // 2x2 subpixel rows
				{
					for (int sx = 0; sx < 2; sx++, r = Vec())
					{        // 2x2 subpixel cols
						for (int s = 0; s < samps; s++)
						{
							// I BELIEVE THIS PRODUCES A TENT FILTER
							double r1 = 2 * erand48(Xi), dx = r1 < 1 ? sqrt(r1) - 1 : 1 - sqrt(2 - r1);
							double r2 = 2 * erand48(Xi), dy = r2 < 1 ? sqrt(r2) - 1 : 1 - sqrt(2 - r2);
							//Vec d = cx * (((sx + .5 + dx) / 2 + x) / w - .5) + cy * (((sy + .5 + dy) / 2 + y) / h - .5) + cam.direction;
							//r = r + SmallPT::radiance(Ray(cam.origin + d * 140, d.norm()), &scene, 0, Xi) * (1. / samps);
							r = r + SmallPT::radiance(camera.generateRay(x, y, sx, sy, dx, dy, Xi), &scene, 0, Xi) * (1. / samps);
						} // Camera rays are pushed ^^^^^ forward to start in interior
						c[i] = c[i] + Vec(r.x, r.y, r.z) * .25;
					}
				}
			}
		}
		SDL_LockMutex(viewer->mutex);
		{
			for (int i = 0; i < w * h; i++)
				viewer->rawSamplesData[i] = viewer->rawSamplesData[i] + c[i];
			viewer->curSPP += viewer->sampleStep;
			viewer->imageNeedsRefresh = true;
		}
		SDL_UnlockMutex(viewer->mutex);
		delete[] c;
		std::cerr << "\rRendering (" << viewer->curSPP << " spp, " <<((double)viewer->sampleStep*w*h) / (double)(SDL_GetTicks() - timeDeltaElapsed) << "k sp/s)"; // print progress

	}
	std::cerr << "\n\ntime needed for rendering: " <<(double)(SDL_GetTicks() - viewer->timeElapsed) / 1000.0 << " s\n"; // print progress
	return 0;
}

void SDLViewer::saveImage()
{
	std::string name = "render" + std::to_string(curSPP) + "spp.bmp";
	Uint32 *pixels = new Uint32[gScreenSurface->w * gScreenSurface->h];
	SDL_Surface* renderedImage = toneMap(pixels);
	//SDL_SaveBMP(renderedImage, name.c_str());
	if(SDL_SaveBMP(renderedImage, name.c_str()) != 0)
	{
	    std::cerr << "\n\n\nSDL_SaveBMP() Failed." << std::endl;
	}

	SDL_FreeSurface(renderedImage);
	delete[] pixels;
}

void SDLViewer::handleEvents(SDL_Event& events, bool& needsUpdate)
{
	while (SDL_PollEvent(&events) != 0)
	{
		// User requests quit
		if (events.type == SDL_QUIT)
			quit = true;
		if (events.type == SDL_MOUSEMOTION || events.type == SDL_MOUSEBUTTONDOWN || events.type == SDL_MOUSEBUTTONUP)
		{
			SDL_Rect buttonRect =
			{ gScreenSurface->w - gButtonDefault->w, gScreenSurface->h - gButtonDefault->h, gButtonDefault->w, gButtonDefault->h };
			bool inside = false;
			int x, y;
			SDL_GetMouseState(&x, &y);
			if (x >= buttonRect.x && y >= buttonRect.y && x >= buttonRect.x && x < gScreenSurface->w && y < gScreenSurface->h)
				inside = true;
			if (events.type == SDL_MOUSEBUTTONUP || events.type == SDL_MOUSEMOTION)
			{
				if(events.type == SDL_MOUSEBUTTONUP && gButtonCurrent == gButtonPressed)
					saveImage();
				if (inside && gButtonCurrent != gButtonHover && (events.type != SDL_MOUSEMOTION || gButtonCurrent != gButtonPressed))
				{
					needsUpdate = true;
					gButtonCurrent = gButtonHover;
				}
				else if (!inside && gButtonCurrent != gButtonDefault)
				{
					needsUpdate = true;
					gButtonCurrent = gButtonDefault;
				}
			}
			else if (events.type == SDL_MOUSEBUTTONDOWN)
			{
				if (inside)
				{
					needsUpdate = true;
					gButtonCurrent = gButtonPressed;
				}
			}
		}
	}
}

SDL_Surface* SDLViewer::toneMap(Uint32* pixels)
{
	int w = gScreenSurface->w;
	int h = gScreenSurface->h;
	SDL_Surface* renderedImage = NULL;
	SDL_LockMutex(mutex);
	{
		for (int i = 0; i < w * h; i++)
		{
			Vec pix = rawSamplesData[i] * (1.0 / (curSPP / sampleStep));
			pixels[i] = 0xFF000000 | (SmallPT::toInt(SmallPT::clamp(pix.x)) << 16) | (SmallPT::toInt(SmallPT::clamp(pix.y)) << 8)
					| SmallPT::toInt(SmallPT::clamp(pix.z));
		}

		renderedImage = SDL_CreateRGBSurfaceFrom((void*) pixels, w, h, 32, 4 * w, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
		imageNeedsRefresh = false;
	}
	SDL_UnlockMutex(mutex);
	return renderedImage;
}
