#include "SmallPT.hpp"
#include "GScene.hpp"
#include "GSphere.hpp"
#include "GTriangle.hpp"
#include "GPolygonObject.hpp"
#include "ObjLoader.hpp"
#include "SDLViewer.hpp"
#include <cstdlib>
#include <cstdio>
#include <string>

/**
 * Parameters as following:
 * 1 path to scene
 * 2, 3 width and height of the render
 * 4 samples per refresh
 * 5 timeSteps for motion blur
 **/
int main(int argc, char *argv[]) {
	int w = 512, h = 512; // image size
	// the following value describes how many pixels are made between every refresh
	int samplePerStep = 4;
	// this value is important for motion Blur depending on the value the blur gets more detailed
	// be careful with this value on complex(memory hungry) scenes like the dragon, since those scenes will get copied 'timeSteps' times
	int timeSteps = 1;
	// obviously the path to the scene file
	std::string pathToScene = "scenes/cornell.obj";
	if( argc > 1 )
		pathToScene = argv[1];
	if( argc > 3 ) {
		w = std::stoi(argv[2]);
		h = std::stoi(argv[3]);
	}

	if( argc > 4 )
		samplePerStep = std::stoi(argv[4]);
	if( argc > 5 )
		timeSteps = std::stoi(argv[5]);
	// the main entry and (render) program starts here:
	SDLViewer view(w, h, pathToScene, samplePerStep, timeSteps);
	view.display();

	return 0;
}
