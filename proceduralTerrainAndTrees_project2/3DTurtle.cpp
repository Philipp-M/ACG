#include <math.h>
#include "3DTurtle.hpp"
#include "Eigen/Dense"
#include "Eigen/Core"


using namespace std;


// draw a 3D line into mesh, the width is defined by line-width, the length by parameter s. 
void _3DTurtle::move(float s){
	
	Vector3d side = (pos.n*pos.dir).colwise().normalize();

	//get vectors

	Vector3d tmp = pos.pos + pos.n*(line_width/2);
	geometry::VertexHandle p1 = mesh.add_vertex(geometry::Point(tmp(0),tmp(1),tmp(2)));		// hexagon top
	tmp = pos.pos + (side * ((2 / 3)*line_width) + ((1 / 6)*pos.n));
	geometry::VertexHandle p2 = mesh.add_vertex(geometry::Point(tmp(0), tmp(1), tmp(2)));	// hexagon top-left
	tmp = pos.pos + (side * ((2 / 3)*line_width) - ((1 / 6)*pos.n));
	geometry::VertexHandle p3 = mesh.add_vertex(geometry::Point(tmp(0), tmp(1), tmp(2)));	// hexagon bottom-left
	Vector3d tmp = pos.pos - pos.n*(line_width / 2);
	geometry::VertexHandle p4 = mesh.add_vertex(geometry::Point(tmp(0), tmp(1), tmp(2)));	// hexagon bottom
	tmp = pos.pos - (side * ((2 / 3)*line_width) - ((1 / 6)*pos.n));
	geometry::VertexHandle p5 = mesh.add_vertex(geometry::Point(tmp(0), tmp(1), tmp(2)));	// hexagon bottom-right
	tmp = pos.pos - (side * ((2 / 3)*line_width) + ((1 / 6)*pos.n));
	geometry::VertexHandle p6 = mesh.add_vertex(geometry::Point(tmp(0), tmp(1), tmp(2)));	// hexagon top-right


	tmp = pos.pos + pos.n*(line_width / 2) + pos.dir*s;
	geometry::VertexHandle t1 = mesh.add_vertex(geometry::Point(tmp(0), tmp(1), tmp(2)));	// hexagon top
	tmp = pos.pos + (side * ((2 / 3)*line_width) + ((1 / 6)*pos.n)) + pos.dir*s;
	geometry::VertexHandle t2 = mesh.add_vertex(geometry::Point(tmp(0), tmp(1), tmp(2)));	// hexagon top-left
	tmp = pos.pos + (side * ((2 / 3)*line_width) - ((1 / 6)*pos.n)) + pos.dir*s;
	geometry::VertexHandle t3 = mesh.add_vertex(geometry::Point(tmp(0), tmp(1), tmp(2)));	// hexagon bottom-left
	Vector3d tmp = pos.pos - pos.n*(line_width / 2) + pos.dir*s;
	geometry::VertexHandle t4 = mesh.add_vertex(geometry::Point(tmp(0), tmp(1), tmp(2)));	// hexagon bottom
	tmp = pos.pos - (side * ((2 / 3)*line_width) - ((1 / 6)*pos.n)) + pos.dir*s;
	geometry::VertexHandle t5 = mesh.add_vertex(geometry::Point(tmp(0), tmp(1), tmp(2)));	// hexagon bottom-right
	tmp = pos.pos - (side * ((2 / 3)*line_width) + ((1 / 6)*pos.n)) + pos.dir*s;
	geometry::VertexHandle t6 = mesh.add_vertex(geometry::Point(tmp(0), tmp(1), tmp(2)));	// hexagon top-right

	//add faces to geometry

	vector<geometry::VertexHandle> face_vhandles;

	face_vhandles.clear();
	face_vhandles.push_back(p1);
	face_vhandles.push_back(p6);
	face_vhandles.push_back(t6);
	face_vhandles.push_back(t1);
	mesh.add_face(face_vhandles);

	face_vhandles.clear();
	face_vhandles.push_back(p6);
	face_vhandles.push_back(p5);
	face_vhandles.push_back(t5);
	face_vhandles.push_back(t6);
	mesh.add_face(face_vhandles);

	face_vhandles.clear();
	face_vhandles.push_back(p5);
	face_vhandles.push_back(p4);
	face_vhandles.push_back(t4);
	face_vhandles.push_back(t5);
	mesh.add_face(face_vhandles);

	face_vhandles.clear();
	face_vhandles.push_back(p4);
	face_vhandles.push_back(p3);
	face_vhandles.push_back(t3);
	face_vhandles.push_back(t4);
	mesh.add_face(face_vhandles);

	face_vhandles.clear();
	face_vhandles.push_back(p3);
	face_vhandles.push_back(p2);
	face_vhandles.push_back(t2);
	face_vhandles.push_back(t3);
	mesh.add_face(face_vhandles);

	face_vhandles.clear();
	face_vhandles.push_back(p2);
	face_vhandles.push_back(p1);
	face_vhandles.push_back(t1);
	face_vhandles.push_back(t2);
	mesh.add_face(face_vhandles);

}


_3DTurtle::_3DTurtle(){
	{
		pos.pos = Vector3d(0.0, 0.0, 0.0);	//starting at origin
		pos.dir = Vector3d(0.0, 1.0, 0.0);	//point up initially
		pos.n   = Vector3d(0.0, 0.0, 1.0);
		line_width = 1.0f;
		color_index = 0;
	}
}