#include <iostream>
// -------------------- OpenMesh
#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/PolyMesh_ArrayKernelT.hh>
#include "INoise.hpp"
#include "DrainagePolygon.hpp"
#include <vector>
  
int main()
{
	// DrainagePolygon test
	DrainagePolygon<double> poly,l,r;
	std::vector<Point2d> polyVerts;
	polyVerts.push_back(Point2d(-10, 10));
	polyVerts.push_back(Point2d(67.997665, 41.755753));
	polyVerts.push_back(Point2d(-10, -10));
	polyVerts.push_back(Point2d(10, -10));
	polyVerts.push_back(Point2d(-55.044014, -123.681725));
	polyVerts.push_back(Point2d(-64.157387, 3.200233));
	polyVerts.push_back(Point2d(10, 0.000000));
	polyVerts.push_back(Point2d(65.284767, 144.163208));
	polyVerts.push_back(Point2d(131.285934, 103.200233));
	polyVerts.push_back(Point2d(135.842606, -71.761581));
	polyVerts.push_back(Point2d(17.357624, -88.159142));
	polyVerts.push_back(Point2d(-112.160889, -26.638300));
	polyVerts.push_back(Point2d(-86.238991, -51.798309));
	polyVerts.push_back(Point2d(-62.237251, 65.681145));
	polyVerts.push_back(Point2d(10.164389, 107.360542));
	poly.setVertices(polyVerts);
	if(poly.split(Point2d(0,0), 1, l, r))
		std::cout << "original Polygon: " << poly << "left: " << l << "right: " << r;
	return 0;
}
