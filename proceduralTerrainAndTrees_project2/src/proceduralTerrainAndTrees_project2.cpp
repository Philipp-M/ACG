#include <iostream>

// -------------------- OpenMesh
#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/PolyMesh_ArrayKernelT.hh>

#include "Common.hpp"
#include "DrainageNetworkTree.hpp"

/*
 * Authors: Philipp Mildenberger, Stefan Spiss
 */

int main()
{
	Mesh drainageBasin;
	Mesh::VertexHandle vhandle[8];

	vhandle[0] = drainageBasin.add_vertex(Mesh::Point(0, 0, 0));
	vhandle[1] = drainageBasin.add_vertex(Mesh::Point(0, 1, 0));
	vhandle[2] = drainageBasin.add_vertex(Mesh::Point(-1, 1, 0));
	vhandle[3] = drainageBasin.add_vertex(Mesh::Point(-1, 0, 0));
	vhandle[4] = drainageBasin.add_vertex(Mesh::Point(1, 1, 0));
	vhandle[5] = drainageBasin.add_vertex(Mesh::Point(1, 0, 0));

	std::vector<Mesh::VertexHandle>  face_vhandles;

	face_vhandles.clear();
	face_vhandles.push_back(vhandle[0]);
	face_vhandles.push_back(vhandle[1]);
	face_vhandles.push_back(vhandle[2]);
	face_vhandles.push_back(vhandle[3]);
	drainageBasin.add_face(face_vhandles);

	face_vhandles.clear();
	face_vhandles.push_back(vhandle[5]);

	face_vhandles.push_back(vhandle[4]);
	face_vhandles.push_back(vhandle[1]);
	face_vhandles.push_back(vhandle[0]);
	drainageBasin.add_face(face_vhandles);


	Mesh::HalfedgeHandle initLink = drainageBasin.halfedge_handle(drainageBasin.edge_handle(0),0);
	DrainageNetworkTree dTree(drainageBasin,initLink,1, 0.33,0.66,0.2,0.1,0.5,0.1);
	Mesh mesh = dTree.buildDrainageNetwork(drainageBasin, initLink);
	try
	{
		if (!OpenMesh::IO::write_mesh(mesh, "output.obj"))
		{
			std::cerr << "Cannot write mesh to file 'output.obj'" << std::endl;
			return 1;
		}
	} catch (std::exception& x)
	{
		std::cerr << x.what() << std::endl;
		return 1;
	}

	return 0;
}
