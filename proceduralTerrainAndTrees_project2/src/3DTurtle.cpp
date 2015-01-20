#include <iostream>
#include "Eigen/Core"
#include "Eigen/Geometry"
#include "3DTurtle.hpp"

using namespace std;

void _3DTurtle::move(float s){
	if (mode)		//polygon_mode activated
		move_polygon(s);
	else			//normal mode activated
		move_normal(s);

	//finally, change position
	pos.pos = pos.pos + pos.dir * s;
}

// draw a 3D line into mesh, the width is defined by line-width, the length by parameter s. 
void _3DTurtle::move_normal(float s){
	Vector3f side = pos.dir.cross(pos.n);
	side.normalize();

	//get vectors

	Vector3f tmp = pos.pos + pos.n*(pos.line_width / 2);
	geometry::VertexHandle p1 = mesh.add_vertex(geometry::Point(tmp(0),tmp(1),tmp(2)));		// hexagon top
	tmp = pos.pos + (side * ((1.f/ 3.f)*pos.line_width) + ((1.f/ 6.f)*pos.n*pos.line_width));
	geometry::VertexHandle p2 = mesh.add_vertex(geometry::Point(tmp(0), tmp(1), tmp(2)));	// hexagon top-left
	tmp = pos.pos + (side * ((1.f/ 3.f)*pos.line_width) - ((1.f/ 6.f)*pos.n*pos.line_width));
	geometry::VertexHandle p3 = mesh.add_vertex(geometry::Point(tmp(0), tmp(1), tmp(2)));	// hexagon bottom-left
	tmp = pos.pos - pos.n*(pos.line_width / 2);
	geometry::VertexHandle p4 = mesh.add_vertex(geometry::Point(tmp(0), tmp(1), tmp(2)));	// hexagon bottom
	tmp = pos.pos - (side * ((1.f/ 3.f)*pos.line_width) + ((1.f/ 6.f)*pos.n*pos.line_width));
	geometry::VertexHandle p5 = mesh.add_vertex(geometry::Point(tmp(0), tmp(1), tmp(2)));	// hexagon bottom-right
	tmp = pos.pos - (side * ((1.f/ 3.f)*pos.line_width) - ((1.f/ 6.f)*pos.n*pos.line_width));
	geometry::VertexHandle p6 = mesh.add_vertex(geometry::Point(tmp(0), tmp(1), tmp(2)));	// hexagon top-right


	tmp = pos.pos + pos.n*(pos.line_width / 2) + pos.dir*s;
	geometry::VertexHandle t1 = mesh.add_vertex(geometry::Point(tmp(0), tmp(1), tmp(2)));	// hexagon top
	tmp = pos.pos + (side * ((1.f/ 3.f)*pos.line_width) + ((1.f/ 6.f)*pos.n*pos.line_width)) + pos.dir*s;
	geometry::VertexHandle t2 = mesh.add_vertex(geometry::Point(tmp(0), tmp(1), tmp(2)));	// hexagon top-left
	tmp = pos.pos + (side * ((1.f/ 3.f)*pos.line_width) - ((1.f/ 6.f)*pos.n*pos.line_width)) + pos.dir*s;
	geometry::VertexHandle t3 = mesh.add_vertex(geometry::Point(tmp(0), tmp(1), tmp(2)));	// hexagon bottom-left
	tmp = pos.pos - pos.n*(pos.line_width / 2) + pos.dir*s;
	geometry::VertexHandle t4 = mesh.add_vertex(geometry::Point(tmp(0), tmp(1), tmp(2)));	// hexagon bottom
	tmp = pos.pos - (side * ((1.f/ 3.f)*pos.line_width) + ((1.f/ 6.f)*pos.n*pos.line_width)) + pos.dir*s;
	geometry::VertexHandle t5 = mesh.add_vertex(geometry::Point(tmp(0), tmp(1), tmp(2)));	// hexagon top-right
	tmp = pos.pos - (side * ((1.f/ 3.f)*pos.line_width) - ((1.f/ 6.f)*pos.n*pos.line_width)) + pos.dir*s;
	geometry::VertexHandle t6 = mesh.add_vertex(geometry::Point(tmp(0), tmp(1), tmp(2)));	// hexagon bottom-right

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


void _3DTurtle::move_polygon(float s){
	//calculate new vertex point
	Vector3f tmp = pos.pos + pos.dir * s;
	geometry::VertexHandle p = mesh.add_vertex(geometry::Point(tmp(0), tmp(1), tmp(2)));

	//put new vertex into vector
	polygon_vertices.push_back(p);

}

// rotation along the dir vector, only change to normal
void _3DTurtle::roll(float a){
	float rad = a* ( (float)M_PI / 180.f);
	AngleAxis<float> aa(rad, pos.dir);
	pos.n = aa * pos.n;
}

//rotation along the side vector
void _3DTurtle::pitch(float a){

	float rad = a* ((float)M_PI / 180.f);
	Vector3f side = pos.dir.cross(pos.n);
	side.normalize();
	AngleAxis<float> aa(rad, side);

	pos.n = aa * pos.n;
	pos.dir = aa* pos.dir;
}

//rotation along the normal
void _3DTurtle::yaw(float a){
	float rad = a* ((float)M_PI / 180.f);
	AngleAxis<float> aa(rad, pos.n);
	pos.dir = aa * pos.dir;
}


void _3DTurtle::to_vertical(){
	pos.dir = Vector3f(0.0, 1.0, 0.0);	//point up 
	pos.n = Vector3f(0.0, 0.0, 1.0);
}

void _3DTurtle::reduce_line_width(){
	pos.line_width = max(0.0f, pos.line_width - (pos.line_width*0.05f));
}

void _3DTurtle::increase_color_index(){
	pos.line_width = min(1.0f, pos.line_width + (pos.line_width*0.05f));
}

void _3DTurtle::top_and_pop(){
	if (!state_stack.empty())
		pos = state_stack.top();
	state_stack.pop();
}

void _3DTurtle::push(){
	state_stack.push(pos);
}

void _3DTurtle::polygon_mode(){
	mode = true;
}

void _3DTurtle::normal_mode(){
	if (mode){		//if polygon-mode is being left, create the polygon-face and add it to mesh
		vector<geometry::VertexHandle> face_vhandles;

		for (auto v : polygon_vertices)
			face_vhandles.push_back(v);

		mesh.add_face(face_vhandles);
		polygon_vertices.clear();
	}
	
	mode = false;
}

geometry _3DTurtle::get_mesh(){
	return geometry(mesh);	//pass copy, just to be safe
}


geometry _3DTurtle::generate(LSystem *ls, int generation){
	this->angle = ls->get_angle();
	string input = ls->apply(generation);
	free(ls);
	return generate(input);
}

geometry _3DTurtle::generate(string input){

	//interpretatation rules taken from "THE ALGORITHMIC BEAUTY OF PLANTS"
	for (char c : input){
		switch (c){
			case 'F':
				move(4.0);
				break;
			case 'f':
				move(2.0);
				break;
			case '+':
				yaw(angle);
				break;
			case '-':
				yaw(-angle);
				break;
			case '^':
				pitch(angle);
				break;
			case '&':
				pitch(-angle);
				break;
			case '\\':
				roll(angle);
				break;
			case '/':
				roll(-angle);
				break;
			case '|':
				yaw(180.0);
				break;
			case '$':
				to_vertical();
				break;
			case '[':
				push();
				break;
			case ']':
				top_and_pop();
				break;
			case '{':
				polygon_mode();
				break;
			case '}':
				normal_mode();
				break;
			case '!':
				reduce_line_width();
				break;
			case '\'':
				pos.color_index++;
				break;
			default:
				break;
		}
	}

	return get_mesh();
}


_3DTurtle::_3DTurtle(float angle){
	this->angle = angle;
	mode = false;
}