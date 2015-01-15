#include <string>
#include <set>
#include <stack>

#ifdef WIN32
#define _USE_MATH_DEFINES
#endif

#include <OpenMesh/Core/Mesh/PolyMesh_ArrayKernelT.hh>
#include "Eigen/Dense"

using namespace std;
using namespace Eigen;

typedef OpenMesh::PolyMesh_ArrayKernelT<> geometry;


typedef struct{
	Vector3d pos;	//position in 3D space
	Vector3d dir;	//direction to draw
	Vector3d n;		//the normal vector, useful for generating the actual faces
	float line_width;
	int color_index;
} state;


class _3DTurtle {
private:
	vector<geometry::VertexHandle> polygon_vertices;
	stack<state> state_stack;
	set<Vector3d> colors;
	state pos;
	geometry mesh;
	double angle;
	bool mode;
public:
	//move foward
	void move(float s);
	void move_normal(float s);
	void move_polygon(float s);

	//change direction
	void roll(double a);
	void pitch(double a);
	void yaw(double a);
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

	//getter
	geometry get_mesh();

	//interpret string
	geometry generate(string input);
	
	_3DTurtle(double angle = 20.0);
};
