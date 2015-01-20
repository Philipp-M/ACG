#include "DrainageNetworkTree.hpp"
//#include <OpenMesh/Core/Geometry/VectorT_inc.hh>

// ------------- DrainageNetworkNode --------------

DrainageNetworkNode::DrainageNetworkNode(const DrainageNetworkTree& fatherTree, DrainageNetworkNode* parent, const Mesh& basinMesh,
		double parametricLength, Mesh::HalfedgeHandle heh) :
		fatherTree(fatherTree), parent(parent), left(nullptr), right(nullptr), hedgeH(heh), parametricLength(parametricLength), shreveOrder(1), valleySlopeL(0), valleySlopeR(0)
{
}

bool DrainageNetworkNode::checkLocalChannelMaintenance(const Mesh& basinMesh, double channelMaintenance) const
{
	Mesh::Scalar dP0 = calculateFaceArea(basinMesh, basinMesh.face_handle(hedgeH));
	Mesh::Scalar dP1 = calculateFaceArea(basinMesh, basinMesh.face_handle(basinMesh.opposite_halfedge_handle(hedgeH)));
	return (dP0 + dP1) / length(basinMesh) < channelMaintenance;
}

bool DrainageNetworkNode::insertChannel(const Mesh& basinMesh, double channelMaintenance)
{
	if (!checkLocalChannelMaintenance(basinMesh, channelMaintenance))
	{
		double junction = fatherTree.meanJunction + fatherTree.deltaJunction * ((double) rand() / RAND_MAX);
		bool leftUpstream = false;
		// check reaches
		DrainageNetworkTree::BasinPart partOfBasin = fatherTree.basinPart(basinMesh.point(basinMesh.from_vertex_handle(hedgeH)));

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

		double exteriorLength = fatherTree.meanLength + fatherTree.deltaLength * ((double) rand() / RAND_MAX);
		parametricLength = junction;
		DrainageNetworkNode* upstream = new DrainageNetworkNode(fatherTree, this, basinMesh, (1.0 - junction)*parametricLength);
		DrainageNetworkNode* exteriorLink = new DrainageNetworkNode(fatherTree, this, basinMesh, exteriorLength);
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

	if (!(left == nullptr && right == nullptr))
	{
		hedgeH = splitLinkInBasinMesh(basinMesh, heh, parametricLength);
		debugMesh(debugi++, basinMesh);
		// also deletes faces
		//basinMesh.delete_edge(tmpEh, false);
		double SD = fatherTree.meanLength * pow((2 * shreveOrder - 1), -0.6);
		double SL = fatherTree.meanLength * pow((2 * left->shreveOrder - 1), -0.6);
		double SR = fatherTree.meanLength * pow((2 * right->shreveOrder - 1), -0.6);
		double EL = acos(SD / SL);
		double ER = acos(SD / SR);
		Mesh::HalfedgeHandle rightLinkEdge = addEdge(basinMesh, heh, -ER);
		debugMesh(debugi++, basinMesh);
		Mesh::HalfedgeHandle leftLinkEdge = addEdge(basinMesh, heh, EL);
		debugMesh(debugi++, basinMesh);
		Mesh::HalfedgeHandle next = basinMesh.next_halfedge_handle(heh);
		basinMesh.remove_edge(basinMesh.edge_handle(heh));
		basinMesh.collapse(next);
		//debugMesh(debugi++, basinMesh);
		left->hedgeH = leftLinkEdge;
		right->hedgeH = rightLinkEdge;
//		left->hedgeH = splitLinkInBasinMesh(basinMesh, leftLinkEdge, left->parametricLength);
//		right->hedgeH = splitLinkInBasinMesh(basinMesh, rightLinkEdge, right->parametricLength);
		//debugMesh(debugi++, basinMesh);
		double ZL1, ZR1, ZL2, ZR2;

		if(SL >= SR) {
			ZL1 = (EL + ER - (asin(SL/(SL+left->getValleySlopeR())) + asin(SR/(SR+right->getValleySlopeL())))) / 2;
			ZR1 = (EL + ER + (asin(SL/(SL+left->getValleySlopeR())) - asin(SR/(SR+right->getValleySlopeL())))) / 2;

			ZL2 = M_PI_2 - (EL + (asin(SL/(SL+left->getValleySlopeL()))) + (asin(SD/(SD+getValleySlopeL())))) / 2;
//			double ZL2_ = M_PI_2 - (EL - (asin(SL/SL+left->getValleySlopeL())) - (asin(SD/SD+getValleySlopeL()))) / 2;

			ZR2 = M_PI_2 - (ER + (asin(SR/(SR+right->getValleySlopeR()))) + (asin(SD/(SD+getValleySlopeR())))) / 2;
//			double ZR2_ = M_PI_2 - (ER - (asin(SR/right->getValleySlopeR())) - (asin(SD/SD+getValleySlopeR()))) / 2;
		} else {
			ZR1 = (ER + EL - (asin(SR / (SR+right->getValleySlopeL())) + asin(SL / (SL+left->getValleySlopeR())))) / 2;
			ZL1 = (ER + EL + (asin(SR / (SR+right->getValleySlopeL())) - asin(SL / (SL+left->getValleySlopeR())))) / 2;

			ZR2 = M_PI_2 - (ER + (asin(SR / (SR+right->getValleySlopeR()))) + (asin(SD / (SD+getValleySlopeR())))) / 2;
//			double ZR2_ = M_PI_2 - (ER - (asin(SR / SR+right->getValleySlopeR())) - (asin(SD / SD+getValleySlopeR()))) / 2;

			ZL2 = M_PI_2 - (EL + (asin(SL / (SL+left->getValleySlopeL()))) + (asin(SD / (SD+getValleySlopeL())))) / 2;
//			double ZL2_ = M_PI_2 - (EL - (asin(SL / SL+left->getValleySlopeL())) - (asin(SD / SD+getValleySlopeL()))) / 2;
		}

		addEdge(basinMesh, left->hedgeH, -ZL1);
		//debugMesh(debugi++, basinMesh);
		addEdge(basinMesh, left->hedgeH, ZL2);
		//debugMesh(debugi++, basinMesh);
		addEdge(basinMesh, right->hedgeH, -ZR2);
		//debugMesh(debugi++, basinMesh);
		left->recalculateBasinMesh(basinMesh, left->hedgeH);
		right->recalculateBasinMesh(basinMesh, right->hedgeH);
	}
	else
	{
		hedgeH = splitLinkInBasinMesh(basinMesh, hedgeH, parametricLength);
	}

}

double DrainageNetworkNode::getValleySlopeL()
{
	if (valleySlopeL == 0)
		valleySlopeL = ((double) rand() / RAND_MAX) * M_PI_2 * 0.9 + 0.01;
	return valleySlopeL;
}

double DrainageNetworkNode::getValleySlopeR()
{
	if (valleySlopeR == 0)
		valleySlopeR = ((double) rand() / RAND_MAX) * M_PI_2 * 0.9 + 0.01;
	return valleySlopeR;
}

DrainageNetworkNode::~DrainageNetworkNode()
{
	if (left != nullptr)
		delete left;
	if (right != nullptr)
		delete right;
}
Mesh::Scalar DrainageNetworkNode::length(const Mesh& basinMesh) const
{
	return basinMesh.calc_edge_length(hedgeH);
}

Mesh::Scalar DrainageNetworkNode::lengthIncSub(const Mesh& basinMesh) const
{
	if (left != nullptr && right != nullptr)
		return length(basinMesh) + left->lengthIncSub(basinMesh) + right->lengthIncSub(basinMesh);
	if (left != nullptr)
		return length(basinMesh) + left->lengthIncSub(basinMesh);
	if (right != nullptr)
		return length(basinMesh) + right->lengthIncSub(basinMesh);
	return length(basinMesh);
}
//int DrainageNetworkNode::recalculateShreveOrder()
//{
//	shreveOrder = 0;
//	// source drainage
//	if (left == nullptr && right == nullptr)
//	{
//		shreveOrder = 1;
//		return 1;
//	}
//	if (left != nullptr)
//		shreveOrder += left->recalculateShreveOrder();
//	if (right != nullptr)
//		shreveOrder += right->recalculateShreveOrder();
//	return shreveOrder;
//}

// ------------- DrainageNetworkNode --------------

DrainageNetworkTree::DrainageNetworkTree(const Mesh& basinMesh, Mesh::HalfedgeHandle initLink, double channelMaintenance, double middleStart, double upperStart,
		double meanJunction, double deltaJunction, double meanLength, double deltaLength) :
		channelMaintenance(channelMaintenance), meanJunction(meanJunction), deltaJunction(deltaJunction), meanLength(meanLength), deltaLength(deltaLength)
{
	root = new DrainageNetworkNode(*this, nullptr, basinMesh, 1, initLink);

	// calculate the Face area of the connected polygons, since the initlink divides the basin, each halfedge HAS to have a connected face!
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
	//Mesh drainageBasin = mesh;
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

