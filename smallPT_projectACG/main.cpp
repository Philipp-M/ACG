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

int main(int argc, char *argv[])
{
	int w = 512, h = 512; // image size
	int samplePerStep = 4;
	int timeSteps = 10;
	if(argc > 2)
	{
		w = std::stoi(argv[1]);
		h = std::stoi(argv[2]);
	}
	if(argc > 3)
		samplePerStep = std::stoi(argv[3]);
	SDLViewer view(w,h, samplePerStep, timeSteps);
	view.display();

	
	return 0;
}
