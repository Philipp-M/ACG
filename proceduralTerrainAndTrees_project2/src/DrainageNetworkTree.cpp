#include "DrainageNetworkTree.hpp"

/*
 * Authors: Philipp Mildenberger, Stefan Spiss
 */


// ------------- DrainageNetworkNode --------------

DrainageNetworkNode::DrainageNetworkNode(const DrainageNetworkTree& parentTree, DrainageNetworkNode* parent, const Mesh& basinMesh,
		double parametricLength, Mesh::HalfedgeHandle heh) :
		parentTree(parentTree), parent(parent), left(nullptr), right(nullptr), hedgeH(heh), parametricLength(parametricLength), shreveOrder(1), valleySlopeL(0), valleySlopeR(0)
{
}
DrainageNetworkNode::~DrainageNetworkNode()
{
	if (left != nullptr)
		delete left;
	if (right != nullptr)
		delete right;
}

bool DrainageNetworkNode::checkLocalChannelMaintenance(const Mesh& basinMesh, double channelMaintenance) const
{
	// checking if channel maintenance reached
	Mesh::Scalar dP0 = calculateFaceArea(basinMesh, basinMesh.face_handle(hedgeH));
	Mesh::Scalar dP1 = calculateFaceArea(basinMesh, basinMesh.face_handle(basinMesh.opposite_halfedge_handle(hedgeH)));
	return (dP0 + dP1) / length(basinMesh) < channelMaintenance;
}

bool DrainageNetworkNode::insertChannel(const Mesh& basinMesh, double channelMaintenance)
{
	if (!checkLocalChannelMaintenance(basinMesh, channelMaintenance))
	{
		// calculate parametricLength for new junction
		int tmp;
		if (((double) rand() / RAND_MAX) < 0.5) {
			tmp = -1;
		}
		else {
			tmp = 1;
		}
		double junction = parentTree.meanJunction + parentTree.deltaJunction * ((double) rand() / RAND_MAX) * tmp;
		bool leftUpstream = false;

		// check reaches
		DrainageNetworkTree::BasinPart partOfBasin = parentTree.basinPart(basinMesh.point(basinMesh.from_vertex_handle(hedgeH)));

		if (partOfBasin == DrainageNetworkTree::BasinPart::MIDDLE)
		{
			if (parent->right == this)
				leftUpstream = true;
		}
		else // UPPER or LOWER
		{
			if (((double) rand() / RAND_MAX) < 0.5)
				leftUpstream = true;
		}

		// calculate parametricLength for new link
		double exteriorLength = parentTree.meanLength + parentTree.deltaLength * ((double) rand() / RAND_MAX) * tmp;

		// setup the new links and update old one
		parametricLength = junction;
		DrainageNetworkNode* upstream = new DrainageNetworkNode(parentTree, this, basinMesh, (1.0 - junction)*parametricLength);
		DrainageNetworkNode* exteriorLink = new DrainageNetworkNode(parentTree, this, basinMesh, exteriorLength);
		upstream->left = left;
		upstream->right = right;
		upstream->shreveOrder = shreveOrder;

		// recalculate Shreve Order
		DrainageNetworkNode* tmpParent = this;
		while (tmpParent != nullptr)
		{
			tmpParent->shreveOrder++;
			tmpParent = tmpParent->parent;
		}

		if (leftUpstream)
		{
			left = upstream;
			right = exteriorLink;
		}
		else
		{
			right = upstream;
			left = exteriorLink;
		}
		return true;
	}

	// recursive call of the function
	if (!(left == nullptr && right == nullptr))
	{
		if (((double) rand() / RAND_MAX) < 0.5)
		{
			if (left->insertChannel(basinMesh, channelMaintenance))
				return true;
			if (right->insertChannel(basinMesh, channelMaintenance))
				return true;
		}
		else
		{
			if (right->insertChannel(basinMesh, channelMaintenance))
				return true;
			if (left->insertChannel(basinMesh, channelMaintenance))
				return true;
		}
	}
	return false;
}

void DrainageNetworkNode::recalculateBasinMesh(Mesh& basinMesh, Mesh::HalfedgeHandle heh)
{
	// if node has children
	if (!(left == nullptr && right == nullptr))
	{
		// split link at parametricLength
		hedgeH = splitLinkInBasinMesh(basinMesh, heh, parametricLength);

		// calculate stream tangents
		double SD = parentTree.meanLength * pow((2 * shreveOrder - 1), -0.6);
		double SL = parentTree.meanLength * pow((2 * left->shreveOrder - 1), -0.6);
		double SR = parentTree.meanLength * pow((2 * right->shreveOrder - 1), -0.6);

		// calculate angles for new links
		double EL = acos(SD / SL);
		double ER = acos(SD / SR);

		// add new link edges
		Mesh::HalfedgeHandle rightLinkEdge = addEdge(basinMesh, heh, -ER);
		Mesh::HalfedgeHandle leftLinkEdge = addEdge(basinMesh, heh, EL);

		// remove unnecessary part of edge and collapse vertices afterwards
		// error happens here, debugMesh before and after remove_edge
		Mesh::HalfedgeHandle next = basinMesh.next_halfedge_handle(heh);
		debugMesh(debugi++, basinMesh);
		basinMesh.remove_edge(basinMesh.edge_handle(heh));
		debugMesh(debugi++, basinMesh);
		basinMesh.collapse(next);

		// set half edges from upper links
		left->hedgeH = leftLinkEdge;
		right->hedgeH = rightLinkEdge;

		// calculation of the angles for the drainage divides
		double ZL1, ZL2, ZR2;
//		double ZR1, ZL2_, ZR2_;
		if(SL >= SR) {
			ZL1 = (EL + ER - (asin(SL/(SL+left->getValleySlopeR())) + asin(SR/(SR+right->getValleySlopeL())))) / 2;
//			ZR1 = (EL + ER + (asin(SL/(SL+left->getValleySlopeR())) - asin(SR/(SR+right->getValleySlopeL())))) / 2;

			ZL2 = M_PI_2 - (EL + (asin(SL/(SL+left->getValleySlopeL()))) + (asin(SD/(SD+getValleySlopeL())))) / 2;
//			ZL2_ = M_PI_2 - (EL - (asin(SL/SL+left->getValleySlopeL())) - (asin(SD/SD+getValleySlopeL()))) / 2;

			ZR2 = M_PI_2 - (ER + (asin(SR/(SR+right->getValleySlopeR()))) + (asin(SD/(SD+getValleySlopeR())))) / 2;
//			ZR2_ = M_PI_2 - (ER - (asin(SR/right->getValleySlopeR())) - (asin(SD/SD+getValleySlopeR()))) / 2;
		} else {
			ZL1 = (ER + EL + (asin(SR / (SR+right->getValleySlopeL())) - asin(SL / (SL+left->getValleySlopeR())))) / 2;
//			ZR1 = (ER + EL - (asin(SR / (SR+right->getValleySlopeL())) + asin(SL / (SL+left->getValleySlopeR())))) / 2;

			ZR2 = M_PI_2 - (ER + (asin(SR / (SR+right->getValleySlopeR()))) + (asin(SD / (SD+getValleySlopeR())))) / 2;
//			ZR2_ = M_PI_2 - (ER - (asin(SR / SR+right->getValleySlopeR())) - (asin(SD / SD+getValleySlopeR()))) / 2;

			ZL2 = M_PI_2 - (EL + (asin(SL / (SL+left->getValleySlopeL()))) + (asin(SD / (SD+getValleySlopeL())))) / 2;
//			ZL2_ = M_PI_2 - (EL - (asin(SL / SL+left->getValleySlopeL())) - (asin(SD / SD+getValleySlopeL()))) / 2;
		}

		// add drainage divides
		addEdge(basinMesh, left->hedgeH, -ZL1);
		addEdge(basinMesh, left->hedgeH, ZL2);
		addEdge(basinMesh, right->hedgeH, -ZR2);

		// calling function recursively
		if(((double) rand() / RAND_MAX) <= 0.5) {
			left->recalculateBasinMesh(basinMesh, left->hedgeH);
			right->recalculateBasinMesh(basinMesh, right->hedgeH);
		} else {
			right->recalculateBasinMesh(basinMesh, right->hedgeH);
			left->recalculateBasinMesh(basinMesh, left->hedgeH);
		}

	}
	else
	{
		// split edge from node without children
		hedgeH = splitLinkInBasinMesh(basinMesh, hedgeH, parametricLength);
	}

}

double DrainageNetworkNode::getValleySlopeL()
{
	// if valleySlopeL not set, calculate random value between 0.01 and PI/2 * 0.1 + 0.01
	if (valleySlopeL == 0)
		valleySlopeL = ((double) rand() / RAND_MAX) * M_PI_2 * 0.9 + 0.01;
	return valleySlopeL;
}

double DrainageNetworkNode::getValleySlopeR()
{
	// if valleySlopeR not set, calculate random value between 0.01 and PI/2 * 0.1 + 0.01
	if (valleySlopeR == 0)
		valleySlopeR = ((double) rand() / RAND_MAX) * M_PI_2 * 0.9 + 0.01;
	return valleySlopeR;
}

Mesh::Scalar DrainageNetworkNode::length(const Mesh& basinMesh) const
{
	return basinMesh.calc_edge_length(hedgeH);
}

// ------------- DrainageNetworkNode --------------

DrainageNetworkTree::DrainageNetworkTree(const Mesh& basinMesh, Mesh::HalfedgeHandle initLink, double channelMaintenance, double middleStart, double upperStart,
		double meanJunction, double deltaJunction, double meanLength, double deltaLength) :
		channelMaintenance(channelMaintenance), meanJunction(meanJunction), deltaJunction(deltaJunction), meanLength(meanLength), deltaLength(deltaLength)
{
	// root node of tree
	root = new DrainageNetworkNode(*this, nullptr, basinMesh, 1, initLink);

	// calculate lines for basin parts
	OpenMesh::Vec3f direction = basinMesh.calc_edge_vector(initLink).normalize();
	OpenMesh::Vec3f directionPerpend = direction % OpenMesh::Vec3f(0, 0, 1);
	upperLine[0] = direction * basinMesh.calc_edge_length(initLink) * upperStart;
	upperLine[1] = upperLine[0] + directionPerpend;
	middleLine[0] = direction * basinMesh.calc_edge_length(initLink) * middleStart;
	middleLine[1] = upperLine[0] + directionPerpend;
}

DrainageNetworkTree::~DrainageNetworkTree()
{
	if (root != nullptr)
		delete root;
}

Mesh DrainageNetworkTree::buildDrainageNetwork(const Mesh& mesh, Mesh::HalfedgeHandle initLink)
{
	Mesh calculatedMesh;
	do
	{
		calculatedMesh = mesh;
		root->recalculateBasinMesh(calculatedMesh, initLink);
		debugMesh(debugi++, calculatedMesh);
	} while(root->insertChannel(calculatedMesh, channelMaintenance));
	return calculatedMesh;
}

DrainageNetworkTree::BasinPart DrainageNetworkTree::basinPart(const Mesh::Point& point) const
{
	// calculation in which part the given point is
	double orientUpper = (upperLine[1].values_[0] - upperLine[0].values_[0]) * (point.values_[1] - upperLine[0].values_[1])
			- (upperLine[1].values_[1] - upperLine[0].values_[1]) * (point.values_[0] - upperLine[0].values_[0]);
	if (orientUpper < 0)
	{
		double orientMiddle = (middleLine[1].values_[0] - middleLine[0].values_[0]) * (point.values_[1] - middleLine[0].values_[1])
				- (middleLine[1].values_[1] - middleLine[0].values_[1]) * (point.values_[0] - middleLine[0].values_[0]);
		if (orientMiddle < 0)
			return BasinPart::LOWER;
		return BasinPart::MIDDLE;
	}
	return BasinPart::UPPER;
}

