#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>
#include <string>
#include <set>
#include <stack>

using namespace std;

typedef OpenMesh::TriMesh_ArrayKernelT<OpenMesh::DefaultTraits> geometry;

typedef struct{
	float x;
	float y;
	float z;
	float azimuth;
	float attidue;
} position;

typedef struct{
	float r;
	float g;
	float b;
} color;

class _3DTurtle {
private:
	set<color> colors;
	int color_index;
	float line_width;
	position pos;		
	stack<position> state_stack;
	
	//move foward
	void move(float s);

	//change direction
	void roll(float a);
	void pitch(float a);
	void yaw(float a);

	//change drawing attributes
	void reduce_line_width();
	void increase_color_index();
public:
	_3DTurtle() { 
		pos = { 0.0, 0.0, 0.0, 0.0, 0.0 }; 
		line_width = 1.0f;
		color_index = 0;
	}



	geometry render(string input);

};
