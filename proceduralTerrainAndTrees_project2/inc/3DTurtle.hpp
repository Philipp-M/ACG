#pragma once
#include <string>
#include <set>
#include <stack>
#include <OpenMesh/Core/Mesh/PolyMesh_ArrayKernelT.hh>
#include "Eigen/Dense"

using namespace std;
using namespace Eigen;

typedef OpenMesh::PolyMesh_ArrayKernelT<> geometry;


struct state {
	Vector3f pos = Vector3f(0.0, 0.0, 0.0) ;	//	position in 3D space
	Vector3f dir = Vector3f(0.0, 1.0, 0.0);		//	direction to draw
	Vector3f n = Vector3f(0.0, 0.0, 1.0);;		//	the normal vector, useful for generating the actual faces
	float line_width = 0.75f;					//	the diameter of the drawn tubes
	int color_index = 0;						//	the color for new faces
};


class _3DTurtle {
private:
	vector<geometry::VertexHandle> polygon_vertices;
	stack<state> state_stack;
	set<Vector3f> colors;
	state pos;
	geometry mesh;
	float angle;
	bool mode;

	//move foward
	void move(float s);
	void move_normal(float s);
	void move_polygon(float s);

	//change direction
	void roll(float a);
	void pitch(float a);
	void yaw(float a);
	void to_vertical();

	//change drawing attributes
	void reduce_line_width();
	void increase_color_index();

	//stack operations
	void top_and_pop();
	void push();

	//switch modes
	void polygon_mode();
	void normal_mode();

public:
	//getter
	geometry get_mesh();

	//interpret string
	geometry generate(string input);
	
	_3DTurtle(float angle);
};
