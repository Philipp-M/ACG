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
	std::string pathToScene = "scenes/cornell.obj";
	if(argc > 1)
		pathToScene = argv[1];
	if(argc > 3)
	{
		w = std::stoi(argv[2]);
		h = std::stoi(argv[3]);
	}

	if(argc > 4)
		samplePerStep = std::stoi(argv[4]);
	SDLViewer view(w,h, pathToScene ,samplePerStep);
	view.display();

	return 0;
}
