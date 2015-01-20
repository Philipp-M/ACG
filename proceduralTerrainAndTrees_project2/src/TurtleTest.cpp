#include <iostream>
#include <OpenMesh/Core/IO/MeshIO.hh>
#include "3DTurtle.hpp"
#include "Lsystem.hpp"

#define TURTLE

int main(int argc, char* argv[]){
	
	_3DTurtle turtle;
	turtle.generate(Stochastic_LSystem::from_file("lsystems/flower2.lsystem"),3);
	geometry g = turtle.get_mesh();


	if (!OpenMesh::IO::write_mesh(g, "turtle.obj")){
		std::cerr << "Cannot write mesh to file 'turtle.obj'" << std::endl;
		return 1;
	}

	return 0;
}