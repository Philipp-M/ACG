#pragma once
#include "DrainageBasin.hpp"
#include "DrainagePolygon.hpp"
#include <vector>

class DrainageNetworkNode
{
	friend class DrainageBasin;
	friend class DrainageNetworkTree;
private:
	const DrainageNetworkTree& fatherTree;
	DrainageNetworkNode* parent;
	DrainageNetworkNode* left;
	DrainageNetworkNode* right;
	/**
	 * central data structure, that contains all connected DrainagePolygons
	 * and the link endpoints(vertices of the first halfedge, where the from vertice is the downstream vertice)
	 * the (as noted in OpenMesh) direction is stored in the first halfedge,
	 * where as the direction points to the triburary links(if any)
	 */
	/**
	 * todo alle Winkel der slope valleys speichern...(welche zufällig bestimmt werden, abhängig von der Höhe(distribution))
	 */
	Mesh::HalfedgeHandle hedgeH;
	double parametricLength;
	int shreveOrder;
	double valleySlopeL;
	double valleySlopeR;
	/**
	 * returns the length of this link
	 */
	Mesh::Scalar length(const Mesh& basinMesh) const;

	bool checkLocalChannelMaintenance(const Mesh& basinMesh, double channelMaintenance) const;
	/**
	 * returns the length of this link and recursively all lengths of its child links
	 */
	Mesh::Scalar lengthIncSub(const Mesh& basinMesh) const;
	/**
	 * @deprecated
	 * recalculates the Shreve Order of this link and all child links
	 */
	//int recalculateShreveOrder() __attribute_deprecated__;
	/**
	 *
	 */
	bool insertChannel(const Mesh& basinMesh, double channelMaintenance);
	/**
	 *
	 */
	void recalculateBasinMesh(Mesh& basinMesh, Mesh::HalfedgeHandle heh);

	double getValleySlopeL();
	double getValleySlopeR();
public:
	DrainageNetworkNode(const DrainageNetworkTree& fatherTree, DrainageNetworkNode* parent, const Mesh& basinMesh, double parametricLength, Mesh::HalfedgeHandle heh = Mesh::HalfedgeHandle());

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
	 * middleStart + upperStart has to be < 1!
	 */
	DrainageNetworkTree(const Mesh& basinMesh, Mesh::HalfedgeHandle initLink, double channelMaintenance, double middleStart, double upperStart,
			double meanJunction, double deltaJunction, double meanLength, double deltaLength);
	~DrainageNetworkTree();
	Mesh buildDrainageNetwork(const Mesh& mesh, Mesh::HalfedgeHandle initLink);
	//void add node
	BasinPart basinPart(const Mesh::Point& point) const;
};
