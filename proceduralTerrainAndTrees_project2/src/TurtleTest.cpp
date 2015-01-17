#include <iostream>
#include <math.h>
#include <OpenMesh/Core/IO/MeshIO.hh>
#include "3DTurtle.hpp"
#include "Lsystem.hpp"

int main(int argc, char* argv[]){
	
	Deterministic_LSystem ls = Deterministic_LSystem::from_file("lsystems/flower.lsystem");
	_3DTurtle turtle(ls.get_angle());
	string bush = ls.apply(1);
	//cout << bush << endl;
	turtle.generate(bush);
	
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