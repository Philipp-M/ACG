#pragma once

#include <OpenMesh/Core/Mesh/PolyMesh_ArrayKernelT.hh>
#include <OpenMesh/Core/IO/MeshIO.hh>
#include <string>

/*
 * Authors: Philipp Mildenberger, Stefan Spiss
 */

struct MyTraits : public OpenMesh::DefaultTraits
{
	VertexAttributes(OpenMesh::Attributes::Status);
	EdgeAttributes(OpenMesh::Attributes::Status);
	FaceAttributes(OpenMesh::Attributes::Status);
	HalfedgeAttributes(OpenMesh::Attributes::Status);
};

typedef OpenMesh::PolyMesh_ArrayKernelT<MyTraits> Mesh;

static int debugi = 0;

/*
 * method to debug the mesh, writes a obj file!
 */
inline void debugMesh(int i, const Mesh& mesh)
{
	try
	{
		char buffer[1000];
		sprintf(buffer, "output%i.obj", i);
		if (!OpenMesh::IO::write_mesh(mesh, buffer))
		{
			std::cerr << "Cannot write mesh to file 'output.obj'" << std::endl;
			return;
		}
	} catch (std::exception& x)
	{
		std::cerr << x.what() << std::endl;
		return;
	}
}

/**
 * calculates the area of a face,
 * the face may only use x and y coordinates thus z has to be zero for all contributing vertices
 */
inline Mesh::Scalar calculateFaceArea(const Mesh& mesh, Mesh::FaceHandle fh)
{
	Mesh::FaceHalfedgeIter fh_it = mesh.cfh_iter(fh);
	Mesh::Scalar retVal = 0;
	for (; fh_it.is_valid(); ++fh_it)
	{
		const Mesh::Point& p0 = mesh.point(mesh.from_vertex_handle(*fh_it));
		const Mesh::Point& p1 = mesh.point(mesh.to_vertex_handle(*fh_it));
		retVal += p0[0] * p1[1] - p1[0] * p0[1];
	}
	return retVal * 0.5;
}

/*
 * splits the link given as the HalfedgeHandle at the parametricLength
 */
inline Mesh::HalfedgeHandle splitLinkInBasinMesh(Mesh& mesh, Mesh::HalfedgeHandle heh, double paramtricLength)
{
	double edgeLength = mesh.calc_edge_length(heh) * paramtricLength;
	OpenMesh::Vec3f edgeDir = mesh.calc_edge_vector(heh).normalize();
	Mesh::VertexHandle vh = mesh.add_vertex(mesh.point(mesh.from_vertex_handle(heh)) + edgeDir * edgeLength);
	mesh.split_edge(mesh.edge_handle(heh), vh);
	Mesh::HalfedgeHandle retVal = mesh.prev_halfedge_handle(heh);
	return retVal;
}

/*
 * add edge starting at the start point of the halfedge "heh" splitting the polygon with the angle "angle"
 */
inline Mesh::HalfedgeHandle addEdge(Mesh& mesh, Mesh::HalfedgeHandle heh, double angle)
{
	const double EPSILON = 10e-5;

	// vector for the edge direction
	OpenMesh::Vec3f edgeDir = mesh.calc_edge_vector(heh).normalize();

	// absolute angel of the edge
	double angleEdge = acos(edgeDir[0]) * (edgeDir[1] >= 0 ? 1 : -1);

	// direction vector for new edge
	OpenMesh::Vec3f dir = OpenMesh::Vec3f(cos(angle + angleEdge), sin(angle + angleEdge), 0);

	// getting the correct face
	Mesh::HalfedgeHandle halfedge = (angle < 0 ? mesh.opposite_halfedge_handle(heh) : heh);
	Mesh::FaceHandle fh = mesh.face_handle(halfedge);

	Mesh::FaceHalfedgeIter fh_it = mesh.fh_iter(fh);
	double minLength = 10e20;
	Mesh::Point pNew;
	Mesh::HalfedgeHandle eNew;
	Mesh::HalfedgeHandle startHalfEdge;

	// two points on the line where the new edge should be
	Mesh::Point pe1 = mesh.point(mesh.from_vertex_handle(heh));
	Mesh::Point pe2 = pe1 + dir;

	// iterating over all edges and check if intersection between the line through that edge
	// and the line of the new edge
	// if intersection -> check if in face and if its the nearest intersection
	// if that is the case -> safe point and edge
	for (; fh_it.is_valid(); ++fh_it)
	{
		if(mesh.point(mesh.to_vertex_handle(*fh_it)) == pe1)
			startHalfEdge = *fh_it;
		Mesh::Point p1 = mesh.point(mesh.from_vertex_handle(*fh_it));
		Mesh::Point p2 = mesh.point(mesh.to_vertex_handle(*fh_it));
		double denom = (p1[0] - p2[0]) * (pe1[1] - pe2[1]) - (p1[1] - p2[1]) * (pe1[0] - pe2[0]);
		if (denom == 0)
			continue;
		Mesh::Point pInt = Mesh::Point(((p1[0] * p2[1] - p1[1] * p2[0]) * (pe1[0] - pe2[0]) - (p1[0] - p2[0]) * (pe1[0] * pe2[1] - pe1[1] * pe2[0])) / denom,
				((p1[0] * p2[1] - p1[1] * p2[0]) * (pe1[1] - pe2[1]) - (p1[1] - p2[1]) * (pe1[0] * pe2[1] - pe1[1] * pe2[0])) / denom, 0);
		if (((pInt[0] <= p1[0] && pInt[0] >= p2[0]) || (pInt[0] >= p1[0] && pInt[0] <= p2[0]))
				&& ((pInt[1] <= p1[1] && pInt[1] >= p2[1]) || (pInt[1] >= p1[1] && pInt[1] <= p2[1]))
				&& !((pInt[0] <= pe1[0] + EPSILON && pInt[0] >= pe1[0] - EPSILON) && (pInt[1] <= pe1[1] + EPSILON && pInt[1] >= pe1[1] - EPSILON)))
		{
			double length = (pInt - pe1).length();
			if (length < minLength)
			{
				minLength = length;
				pNew = pInt;
				eNew = *fh_it;
			}
		}
	}

	// if no intersection found, exit program
	if(minLength == 10e20)
	{
		std::cerr << "Error: No intersection!!!" << std::endl;
		exit(-1);
	}

	// split edge where intersection is and insert new edge
	mesh.split_edge(mesh.edge_handle(eNew), mesh.add_vertex(pNew));
	Mesh::HalfedgeHandle toHalfEdge = eNew;
	Mesh::HalfedgeHandle ehr = mesh.insert_edge(startHalfEdge, toHalfEdge);

	return ehr;
}
