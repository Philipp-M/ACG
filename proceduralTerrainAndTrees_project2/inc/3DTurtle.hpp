#include <string>
#include <set>
#include <stack>
#include <OpenMesh/Core/Mesh/PolyMesh_ArrayKernelT.hh>
#include "Eigen/Dense"

using namespace std;
using namespace Eigen;

typedef OpenMesh::PolyMesh_ArrayKernelT<> geometry;


typedef struct{
	Vector3d pos;	//position in 3D space
	Vector3d dir;	//direction to draw
	Vector3d n;		//the normal vector, useful for generating the actual faces
} position;


//TODO: Add functionaliy for direct polygon draw mode: 
//			o Some data structure to store the verteces
//			o Store stuff into it, then once polygon-mode is turned of, put everything into face


class _3DTurtle {
private:
	set<Vector3d> colors;
	int color_index;
	float line_width;
	position pos;		
	stack<position> state_stack;
	geometry mesh;
	
	//move foward
	void move(float s);

	//change direction
	void roll(float a);
	void pitch(float a);
	void yaw(float a);

	//change drawing attributes
	void reduce_line_width();
	void increase_color_index();

	//stack operations
	void top_and_pop();
	void push();

public:
	_3DTurtle();



	geometry render(string input);

};
