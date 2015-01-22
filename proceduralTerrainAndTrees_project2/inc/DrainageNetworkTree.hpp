#pragma once
#include <vector>
#include "Common.hpp"

/*
 * Authors: Philipp Mildenberger, Stefan Spiss
 */

class DrainageNetworkTree;
class DrainageNetworkNode
{
	friend class DrainageNetworkTree;
private:
	/**
	 * not very clean, but handy to have a handle to the tree that this node contains
	 */
	const DrainageNetworkTree& parentTree;
	/**
	 * The father link, null if root node
	 */
	DrainageNetworkNode* parent;
	/**
	 * The two sublinks
	 */
	DrainageNetworkNode* left;
	DrainageNetworkNode* right;
	/**
	 * central data structure, that contains all connected DrainagePolygons
	 * and the link endpoints(vertices, where the 'from vertice' is the downstream vertice)
	 * the direction is described with the halfedge's direction
	 */
	Mesh::HalfedgeHandle hedgeH;

	double parametricLength;
	int shreveOrder;
	double valleySlopeL;
	double valleySlopeR;
	/**
	 * returns the absolute length of this link
	 */
	Mesh::Scalar length(const Mesh& basinMesh) const;
	/**
	 * checks the channel maintenance for the link, if it returns true the link is saturated
	 * and don't need any further splits
	 */
	bool checkLocalChannelMaintenance(const Mesh& basinMesh, double channelMaintenance) const;
	/**
	 * checks if there is a link where the channelMaintenance is smaller than A/L if it is smaller a channel
	 * will be inserted and true will be returned, if no channel is inserted(and false returned), the drainage system is saturated
	 * So it is finished then with inserting.
	 */
	bool insertChannel(const Mesh& basinMesh, double channelMaintenance);
	/**
	 * The main recursive function to recalculate the mesh
	 */
	void recalculateBasinMesh(Mesh& basinMesh, Mesh::HalfedgeHandle heh);
	/**
	 * simple getter functions, the first time they are called, they generate a random value for the valley slope tangent angle
	 */
	double getValleySlopeL();
	double getValleySlopeR();
public:
	DrainageNetworkNode(const DrainageNetworkTree& parentTree, DrainageNetworkNode* parent, const Mesh& basinMesh, double parametricLength, Mesh::HalfedgeHandle heh = Mesh::HalfedgeHandle());

	~DrainageNetworkNode();
};
/**
 * Binary Tree structure
 */
class DrainageNetworkTree
{
	friend class DrainageNetworkNode;
private:
	DrainageNetworkNode* root;
	Mesh::Point middleLine[2];
	Mesh::Point upperLine[2];
	double channelMaintenance;
	double meanJunction;
	double deltaJunction;
	double meanLength;
	double deltaLength;

public:
	enum class BasinPart {UPPER, MIDDLE, LOWER};
	/**
	 * @param middleStart, at this percentual value, the middle part of the stream starts(everything lower is the lower part)
	 * @param upperStart, at this percentual value, the upper part of the stream starts(everything lower is the middle part)
	 * @param channelMaintenance the constant, that describes the humidity of the drainage basin
	 * @param meanJunction mean length where the junction is placed
	 * @param deltaJunction perturbating value for the junction position
	 * @param meanLength mean length for the exterior link length
	 * @param deltaLength perturbating value for the exterior link length
	 * middleStart + upperStart has to be < 1!
	 */
	DrainageNetworkTree(const Mesh& basinMesh, Mesh::HalfedgeHandle initLink, double channelMaintenance, double middleStart, double upperStart,
			double meanJunction, double deltaJunction, double meanLength, double deltaLength);
	~DrainageNetworkTree();
	/**
	 * given a drainage basin described in the documentation(a polygon splitted by a initial link)
	 * the stream network will be build
	 * returns the finished mesh
	 */
	Mesh buildDrainageNetwork(const Mesh& mesh, Mesh::HalfedgeHandle initLink);
	/**
	 * function to determine the part of the drainage basin based on a given point
	 */
	BasinPart basinPart(const Mesh::Point& point) const;
};
