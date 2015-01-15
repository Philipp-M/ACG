#pragma once

#include "DrainagePolygon.hpp"
#include <vector>

class DrainageNetworkNode
{
private:
	DrainageNetworkNode* left;
	DrainageNetworkNode* right;
	Point2d upperPoint;
	Point2d lowerPoint;
	std::vector<DrainagePolygon<double> > drainagePolygons;
	double length;
	int shreveOrder;
public:

};
/**
 * Binary Tree structure
 */
class DrainageNetworkTree
{
private:
	DrainageNetworkNode* root;
public:
	void build();
};
