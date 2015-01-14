
#ifdef WIN32
#define _USE_MATH_DEFINES
#endif
#include <iostream>
#include <math.h>
#include <OpenMesh/Core/IO/MeshIO.hh>
#include "3DTurtle.hpp"

int main(int argc, char* argv[]){
	_3DTurtle turtle;

	//simple instructions

	turtle.move(4.0);
	turtle.yaw(90.0);
	turtle.move(1.0);
	turtle.pitch(45.0);
	turtle.move(2.0);
	turtle.polygon_mode();
	turtle.move(2.0); 
	turtle.yaw(90.0);
	turtle.move(2.0); 		
	turtle.yaw(90.0);
	turtle.move(2.0);
	turtle.yaw(90.0);
	turtle.move(2.0);
	turtle.normal_mode();

	geometry g = turtle.get_mesh();

	try
	{
		if (!OpenMesh::IO::write_mesh(g, "turtle.obj"))
		{
			std::cerr << "Cannot write mesh to file 'turtle.obj'" << std::endl;
			return 1;
		}
	}
	catch (std::exception& x)
	{
		std::cerr << x.what() << std::endl;
		return 1;
	}
		
	return 0;

}