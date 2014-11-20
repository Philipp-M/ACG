#include "GBVHAccelStruct.hpp"
#include "GObject.hpp"
#include <iostream>
#include <queue>
#include <limits>
#include <stdint.h>

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
	OctreeNode() : isLeaf(true), data(NULL), bbox(Vec3(),Vec3()) { for(size_t i = 0; i < 8; i++) child[i] = NULL; }
	~OctreeNode() { for (uint8_t i = 0; i < 8; ++i) if (child[i] != NULL) delete child[i]; }
	/**
	 * builds the bounding box, based on the child nodes(which also will be build).
	 * should only be executed of the root node
	 */
	void buildBoundingBox() { buildBB(this); }
private:
	/**
	 * implementation of the above told method,
	 * works recursively
	 */
	const GBoundingBox& buildBB(OctreeNode* node)
	{
		if(node->isLeaf)
		{
			node->bbox = node->data->createBoundingBox();
			return node->bbox; // should never be NULL, if so the program has to crash!
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

	Octree(const Vec3& minBB, const Vec3& maxBB) : min(minBB), max(maxBB) {}
	void insert(const GObject* obj)
	{
		insert(&root, obj, obj->getCentroid(), min, max);
	}
	void build()
	{
		root.buildBoundingBox();
	}
	// following was almost taken 1:1 from scratchapixel, since I have not seen a point to rewrite it...
	struct QueueElement
	{
		const OctreeNode *node; // octree node held by this node in the tree
		double t; // used as key
		QueueElement(const OctreeNode *n, double thit) : node(n), t(thit) {}
		// comparator is > instead of < so priority_queue behaves like a min-heap
		friend bool operator < (const QueueElement &a, const QueueElement &b) { return a.t > b.t; }
	};
private:
	void insert(OctreeNode* node, const GObject* obj, const Vec3& centroid, const Vec3& minBB, const Vec3& maxBB)
	{
		/**
		 * if the object ends in a leaf, the object will either be added to the node(if the leaf was empty)
		 * otherwise both objects will be put in one of the subnodes/leafs...
		 */
		if (node->isLeaf)
		{
			if (node->data == NULL)
				node->data = obj;
			else
			{
				node->isLeaf = false;
				if(node->data != NULL) // should always be not NULL, but just to be safe, its not that expensive
				{
					Vec3 centroid_d = node->data->getCentroid();
					double e = 1e-6;
					// check if the centroids are (almost) the same.
					if(centroid.x <= centroid_d.x*(1+e) && centroid.x >= centroid_d.x*(1-e) &&
							centroid.y <= centroid_d.y*(1+e) && centroid.y >= centroid_d.y*(1-e) &&
							centroid.z <= centroid_d.z*(1+e) && centroid.z >= centroid_d.z*(1-e))
					{
						centroid_d = centroid*0.999999; // if two centroids are (almost) the same, recompute the centroid, else this ends in endless recursion
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
			/**
			 * some quite nice tricks are used from the scratchapixel website, as following:
			 */
			uint8_t child_id = 0;
			Vec3 nodeCentroid = (minBB + maxBB) * 0.5f;
			if (nodeCentroid.z < centroid.z)
				child_id += 4;
			if (nodeCentroid.y < centroid.y)
				child_id += 2;
			if (nodeCentroid.x < centroid.x)
				child_id++;
			// compute the childbounds
			Vec3 childBoundMin;
			Vec3 childBoundMax;
			childBoundMin.z = (child_id & 4) ? nodeCentroid.z : minBB.z;
			childBoundMax.z = (child_id & 4) ? maxBB.z : nodeCentroid.z;
			childBoundMin.y = (child_id & 2) ? nodeCentroid.y : minBB.y;
			childBoundMax.y = (child_id & 2) ? maxBB.y : nodeCentroid.y;
			childBoundMin.x = (child_id & 1) ? nodeCentroid.x : minBB.x;
			childBoundMax.x = (child_id & 1) ? maxBB.x : nodeCentroid.x;
			//recursively add the object in a subnode/leaf(where its centroid was inside the bounding box)
			if(node->child[child_id] == NULL)
				node->child[child_id] = new OctreeNode;
			insert(node->child[child_id], obj, centroid, childBoundMin, childBoundMax);

		}
	}
	Vec3 min;
	Vec3 max;
};


GBVHAccelStruct::~GBVHAccelStruct()
{
	delete octree;
}

GBVHAccelStruct::GBVHAccelStruct(const std::vector<GObject*>& objects_)
{
	GBoundingBox bbox = GBoundingBox(Vec3(), Vec3());
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
	// check if the BVH Bounding Box is intersected
	if (!octree->root.bbox.intersect(ray))
		return false;
	double tMin = std::numeric_limits<double>::infinity();
	std::priority_queue<Octree::QueueElement> queue;
	queue.push(Octree::QueueElement(&octree->root, 0));
	RayIntPt isectDataCurrent;
	/**
	 * explaining following code is quite difficult without a picture, like the figure 7.
	 * at this website(http://www.scratchapixel.com/old/lessons/3d-basic-lessons/lesson-12-introduction-to-acceleration-structures/bounding-volume-hierarchy-bvh-part-2/)
	 * the website shows also, that most of the code is from there, since I have not seen the point, to fully rewrite it...
	 * there may be a more efficient solution than the priority queue, but since this is not a "speed" hack project,
	 * I left the readable solution of scratchapixel...
	 *
	 * Although I try to explain the code a little bit
	 */
	/**
	 * while there is an intersection with a bounding box, that is 'nearer' than the last intersection point(if any)
	 * do the following(inside the loop)
	 */
	while (!queue.empty() && queue.top().t < tMin)
	{
		// take the nearest BoundingVolume, that was intersected
		const OctreeNode *node = queue.top().node;
		queue.pop();
		// if it is a leaf check if the intersection Point( if any) is nearer than the current(if any)
		// and update the resulting intersection point
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
		// if it was no leaf, check all the sub-bounding boxes, which can be a maximum of 8 ones(because an Octree was used to build up the Hierarchy)
		else
		{
			for (uint8_t i = 0; i < 8; ++i)
			{
				if (node->child[i] != NULL)
				{
					double tChild;
					// if it was hit put it to the priority queue
					if (node->child[i]->bbox.intersect(ray,tChild))
						queue.push(Octree::QueueElement(node->child[i], tChild));
				}
			}
		}
	}
	// if no Object was hit, the distance remains infinity, so the intersect functin will return false
	if(tMin < std::numeric_limits<double>::infinity())
		return true;
	else
		return false;
}
