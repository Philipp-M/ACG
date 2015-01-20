//=============================================================================
//
//                               OpenMesh
//      Copyright (C) 2001-2005 by Computer Graphics Group, RWTH Aachen
//                           www.openmesh.org
//
//-----------------------------------------------------------------------------
//
//                                License
//
//   This library is free software; you can redistribute it and/or modify it
//   under the terms of the GNU Library General Public License as published
//   by the Free Software Foundation, version 2.
//
//   This library is distributed in the hope that it will be useful, but
//   WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//   Library General Public License for more details.
//
//   You should have received a copy of the GNU Library General Public
//   License along with this library; if not, write to the Free Software
//   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//
//-----------------------------------------------------------------------------
//
//   $Revision: 83 $
//   $Date: 2009-02-27 17:31:45 +0100 (Fri, 27 Feb 2009) $
//
//=============================================================================

#include <iostream>
// -------------------- OpenMesh
#include <OpenMesh/Core/IO/MeshIO.hh>
#include "DrainageNetworkTree.hpp"

// ----------------------------------------------------------------------------
// Build a simple cube and write it to std::cout

int main()
{
//  MyMesh mesh;
//
//  // generate vertices
//
	Mesh drainageBasin;
	Mesh::VertexHandle vhandle[8];
//
	vhandle[0] = drainageBasin.add_vertex(Mesh::Point(0, 0, 0));
	vhandle[1] = drainageBasin.add_vertex(Mesh::Point(0, 1, 0));
	vhandle[2] = drainageBasin.add_vertex(Mesh::Point(-1, 1, 0));
	vhandle[3] = drainageBasin.add_vertex(Mesh::Point(-1, 0, 0));
	vhandle[4] = drainageBasin.add_vertex(Mesh::Point(1, 1, 0));
	vhandle[5] = drainageBasin.add_vertex(Mesh::Point(1, 0, 0));
	vhandle[6] = drainageBasin.add_vertex(Mesh::Point(1, 0.5, 0));
	vhandle[7] = drainageBasin.add_vertex(Mesh::Point(0.2, 0.55, 0));
	vhandle[8] = drainageBasin.add_vertex(Mesh::Point(1, 0.55, 0));
//  vhandle[6] = mesh.add_vertex(MyMesh::Point( 1,  1, -1));
//  vhandle[7] = mesh.add_vertex(MyMesh::Point(-1,  1, -1));
//
//
//  // generate (quadrilateral) faces
//
	std::vector<Mesh::VertexHandle>  face_vhandles;
//
	face_vhandles.clear();
	face_vhandles.push_back(vhandle[0]);
	face_vhandles.push_back(vhandle[1]);
	face_vhandles.push_back(vhandle[2]);
	face_vhandles.push_back(vhandle[3]);
	drainageBasin.add_face(face_vhandles);
//
	face_vhandles.clear();
	face_vhandles.push_back(vhandle[5]);
	face_vhandles.push_back(vhandle[6]);
	face_vhandles.push_back(vhandle[7]);
	face_vhandles.push_back(vhandle[8]);
	face_vhandles.push_back(vhandle[4]);
	face_vhandles.push_back(vhandle[1]);
	face_vhandles.push_back(vhandle[0]);
	drainageBasin.add_face(face_vhandles);
//
//  face_vhandles.clear();
//  face_vhandles.push_back(vhandle[1]);
//  face_vhandles.push_back(vhandle[0]);
//  face_vhandles.push_back(vhandle[4]);
//  face_vhandles.push_back(vhandle[5]);
//  mesh.add_face(face_vhandles);
//
//  face_vhandles.clear();
//  face_vhandles.push_back(vhandle[2]);
//  face_vhandles.push_back(vhandle[1]);
//  face_vhandles.push_back(vhandle[5]);
//  face_vhandles.push_back(vhandle[6]);
//  mesh.add_face(face_vhandles);
//
//  face_vhandles.clear();
//  face_vhandles.push_back(vhandle[3]);
//  face_vhandles.push_back(vhandle[2]);
//  face_vhandles.push_back(vhandle[6]);
//  face_vhandles.push_back(vhandle[7]);
//  mesh.add_face(face_vhandles);
//
//  face_vhandles.clear();
//  face_vhandles.push_back(vhandle[0]);
//  face_vhandles.push_back(vhandle[3]);
//  face_vhandles.push_back(vhandle[7]);
//  face_vhandles.push_back(vhandle[4]);
//  mesh.add_face(face_vhandles);
//  mesh.

// write mesh to output.obj
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
