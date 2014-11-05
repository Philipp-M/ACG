#include "GBVHAccelStruct.hpp"
#include "GObject.hpp"
#include <iostream>
/**
 * following Octree is just needed for building up the hierarchy.
 * Although the octree is also an aproach for an acceleration structure(not as good as the BVH in most cases btw.),
 * I will not implement it as acceleration structure...
 */
struct OctreeNode
{
	bool isLeaf;
	OctreeNode *child[8];
	const GObject* data;
	OctreeNode() : isLeaf(true), data(NULL) { for(size_t i = 0; i < 8; i++) child[i] = NULL; }
	~OctreeNode() { for (uint8_t i = 0; i < 8; ++i) if (child[i] != NULL) delete child[i]; }
};
class Octree
{
public:
	Octree(const Vec& minBB, const Vec& maxBB) : min(minBB), max(maxBB) {}
	void insert(const GObject* obj)
	{
		insert(&root, obj, obj->getCentroid(), min, max);
	}
	// for debugging:
	void print()
	{
		print(&root, 0);
	}
private:
	void print(OctreeNode* node, int depth)
	{
		for(size_t i = 0; i < 8; i++)
		{
			std::cout << "node " << i << " at depth " << depth << " isLeaf: " << node->isLeaf << " has data: " << node->data << std::endl;
			if(node->child[i] != NULL)
				print(node->child[i], depth+1);
		}
	}
	void insert(OctreeNode* node, const GObject* obj, const Vec& centroid, const Vec& minBB, const Vec& maxBB)
	{
		if (node == NULL || obj == NULL || minBB.x > centroid.x || minBB.y > centroid.y || minBB.z > centroid.z ||
			maxBB.x < centroid.x || maxBB.y < centroid.y || maxBB.z < centroid.z)
			return; // this should not happen...
		//doing some other checks here...
		if (node->isLeaf)
		{
			if (node->data == NULL)
			{
				node->data = obj;

			}
			else
			{
				// todo:
				// what happens if two objects centroids are the same, it would happen in endless recursion, it has to be handled
				node->isLeaf = false;
				if(node->data != NULL) // should always be not NULL, but just to be safe, its not that expensive
				{
					Vec centroid_d = node->data->getCentroid();
					double e = 1e-3;
					if(centroid.x <= centroid_d.x*(1+e) && centroid.x >= centroid_d.x*(1-e) &&
							centroid.y <= centroid_d.y*(1+e) && centroid.y >= centroid_d.y*(1-e) &&
							centroid.z <= centroid_d.z*(1+e) && centroid.z >= centroid_d.z*(1-e))
					{
						centroid_d = centroid*0.9999;
						insert(node, node->data, centroid_d, minBB, maxBB); // insert old object in subnode
					}
					else
						insert(node, node->data, node->data->getCentroid(), minBB, maxBB); // insert old object in subnode
				}
					node->data = NULL;
				insert(node, obj, centroid, minBB, maxBB);
			}
		}
		else
		{
			uint8_t child_id = 0;
			Vec nodeCentroid = (minBB + maxBB) * 0.5f;
			if (nodeCentroid.z < centroid.z)
				child_id += 4;
			if (nodeCentroid.y < centroid.y)
				child_id += 2;
			if (nodeCentroid.x < centroid.x)
				child_id++;
			// compute the childbounds
			Vec childBoundMin;
			Vec childBoundMax;
			childBoundMin.z = (child_id & 4) ? nodeCentroid.z : minBB.z;
			childBoundMax.z = (child_id & 4) ? maxBB.z : nodeCentroid.z;
			childBoundMin.y = (child_id & 2) ? nodeCentroid.y : minBB.y;
			childBoundMax.y = (child_id & 2) ? maxBB.y : nodeCentroid.y;
			childBoundMin.x = (child_id & 1) ? nodeCentroid.x : minBB.x;
			childBoundMax.x = (child_id & 1) ? maxBB.x : nodeCentroid.x;
			if(node->child[child_id] == NULL)
				node->child[child_id] = new OctreeNode;
			insert(node->child[child_id], obj, centroid, childBoundMin, childBoundMax);

		}
	}
	OctreeNode root;
	Vec min;
	Vec max;
};


GBVHAccelStruct::~GBVHAccelStruct()
{
}

GBVHAccelStruct::GBVHAccelStruct(const std::vector<GObject*>& objects_)
{
	GBoundingBox bbox = GBoundingBox(Vec(), Vec());
	// building the Bounding Box of all given objects, for the octree base.
	if (!objects_.empty())
	{
		bbox = objects_[0]->createBoundingBox();
		for (size_t i = 1; i < objects_.size(); i++)
			bbox = bbox + objects_[i]->createBoundingBox();
	}
	Octree octree(bbox.getMin(), bbox.getMax());
	for (size_t i = 0; i < objects_.size(); i++)
		octree.insert(objects_[i]);
	octree.print();
}

GBoundingBox GBVHAccelStruct::calculateBoundingBox(const std::vector<GBoundingBox>& bboxes)
{
	if (!bboxes.empty())
	{
		GBoundingBox bbox = bboxes[0];
		for (size_t i = 1; i < bboxes.size(); i++)
			bbox = bbox + bboxes[i];
		return bbox;
	}
	else
		return GBoundingBox(Vec(), Vec()); // should not happen... hopefully
}
