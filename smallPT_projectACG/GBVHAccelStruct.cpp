#include "GBVHAccelStruct.hpp"
#include "GObject.hpp"
#include <iostream>
#include <queue>
#include <limits>
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
	GBoundingBox bbox;
	OctreeNode() : isLeaf(true), data(NULL), bbox(Vec(),Vec()) { for(size_t i = 0; i < 8; i++) child[i] = NULL; }
	~OctreeNode() { for (uint8_t i = 0; i < 8; ++i) if (child[i] != NULL) delete child[i]; }
	/**
	 * builds the bounding box, based on the child nodes(which also will be build.
	 * should only be executed of the root node
	 */
	void buildBoundingBox() { bbox = buildBB(this); }
private:
	const GBoundingBox& buildBB(OctreeNode* node)
	{
		if(node->isLeaf)
		{
			node->bbox = node->data->createBoundingBox();
			return node->bbox; // should always be not NULL, if so the program has to crash!
		}
		else
		{
			for(size_t i = 0; i < 8; i++)
			{
				if(node->child[i] != NULL)
				{
					if(node->bbox.isEmpty())
						node->bbox = buildBB(node->child[i]);
					else
						node->bbox = node->bbox + buildBB(node->child[i]);
				}
			}
			return node->bbox;
		}
	}

};
class Octree
{
public:
	OctreeNode root;

	Octree(const Vec& minBB, const Vec& maxBB) : min(minBB), max(maxBB) {}
	void insert(const GObject* obj)
	{
		insert(&root, obj, obj->getCentroid(), min, max);
	}
	void build()
	{
		root.buildBoundingBox();
	}
	struct QueueElement
	{
		const OctreeNode *node; // octree node held by this node in the tree
		double t; // used as key
		QueueElement(const OctreeNode *n, double thit) : node(n), t(thit) {}
		// comparator is > instead of < so priority_queue behaves like a min-heap
		friend bool operator < (const QueueElement &a, const QueueElement &b) { return a.t > b.t; }
	};
private:
	void insert(OctreeNode* node, const GObject* obj, const Vec& centroid, const Vec& minBB, const Vec& maxBB)
	{
		if (node == NULL || obj == NULL || minBB.x > centroid.x || minBB.y > centroid.y || minBB.z > centroid.z ||
			maxBB.x < centroid.x || maxBB.y < centroid.y || maxBB.z < centroid.z)
			return; // this should not happen...
		//doing some other checks here...
		if (node->isLeaf)
		{
			if (node->data == NULL)
				node->data = obj;
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
	Vec min;
	Vec max;
};


GBVHAccelStruct::~GBVHAccelStruct()
{
	delete octree;
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
	octree = new Octree(bbox.getMin(), bbox.getMax());
	for (size_t i = 0; i < objects_.size(); i++)
		octree->insert(objects_[i]);
	octree->build();
}

bool GBVHAccelStruct::intersect(const Ray& ray, RayIntPt& intPoint) const
{
	if (!octree->root.bbox.intersect(ray))
		return false;
	double tMin = std::numeric_limits<double>::infinity();
	std::priority_queue<Octree::QueueElement> queue;
	queue.push(Octree::QueueElement(&octree->root, 0));
	RayIntPt isectDataCurrent;
	while (!queue.empty() && queue.top().t < tMin)
	{
		const OctreeNode *node = queue.top().node;
		queue.pop();
		if (node->isLeaf)
		{
			if (node->data->intersect(ray, isectDataCurrent))
			{
				if (isectDataCurrent.distance < tMin)
				{
					tMin = isectDataCurrent.distance;
					intPoint = isectDataCurrent;
				}
			}
		}
		else
		{
			for (uint8_t i = 0; i < 8; ++i)
			{
				if (node->child[i] != NULL)
				{
					double tChild;
					if (node->child[i]->bbox.intersect(ray,tChild))
						queue.push(Octree::QueueElement(node->child[i], tChild));
				}
			}
		}
	}
	if(tMin < std::numeric_limits<double>::infinity())
		return true;
	else
		return false;
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
