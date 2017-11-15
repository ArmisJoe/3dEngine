#ifndef __QUADTREEH_H__
#define __QUADTREEH_H__


#include "GameObject.h"
#include "MathGeoLib/MathGeoLib.h"
#include <list>
#include <map>
//#include "Application.h"
//#include "ModuleCamera3D.h"

//#include <algorithm>

#define MAX_ELEMENTS 1
#define SUBDIVISIONS 8
#define MIN_SIZE 10.0f 
#define DEPTH 20

class QuadtreeNode {
public:
	QuadtreeNode();
	QuadtreeNode(const AABB & limits);
	QuadtreeNode(const AABB &aabb, QuadtreeNode* node);

	virtual ~QuadtreeNode();
	void Insert(GameObject* go);
	void Remove(GameObject* go);
	void Create(const AABB &limits);
	//void Clear();
	void Redistribute();
	void Devide();

	template<typename TYPE>
	void CollectIntersections(std::vector<GameObject*>& objects, const TYPE & primitive) const;

	void CollectIntersectionsFRUSTUM(std::vector<GameObject*>& objects, const Frustum & primitive) const; // frustum has a special way to check collision way more efficient

	AABB GetBox() const { return size; }

private:
	QuadtreeNode* parent = nullptr;
	list<GameObject*> elements;
public:
	AABB size;
	QuadtreeNode* nodes[SUBDIVISIONS];
};

class Quadtree
{
public:
	Quadtree();
	virtual ~Quadtree();
	void SetMaxSize(const AABB& box);
	void Insert(GameObject* go);
	void Erase(GameObject* go);
	void Clear();
	void Divide();
	void Intersections(std::vector<GameObject*>& objects, const AABB &aabb) const;
	void CollectAllNodes(vector<AABB>& nodes);
	void RecursiveNodes(vector<AABB>& nodes, QuadtreeNode* it);
	void ClearNode(QuadtreeNode* point);
	bool Adapt(const AABB &aabb);

public:
	QuadtreeNode* root = nullptr;
	uint max_divisions = DEPTH;
private: 
	vector<GameObject*> static_gos;
	float3 min_point = float3::zero;
	float3 max_point = float3::zero;
};

template<typename TYPE>
inline void QuadtreeNode::CollectIntersections(std::vector<GameObject*>& objects, const TYPE & primitive) const
{
		if (primitive.Intersects(size))
		{
			for (std::list<GameObject*>::const_iterator it = this->elements.begin(); it != this->elements.end(); ++it)
			{
				if (primitive.Intersects((*it)->aabb))
				{
					objects.push_back(*it);
				}

			}
			if (nodes[0]!=nullptr)
			{
				for (int i = 0; i < SUBDIVISIONS; ++i)
					nodes[i]->CollectIntersections(objects, primitive);
			}

		}
}

// frustum has a special way to check collision way more efficient
inline void QuadtreeNode::CollectIntersectionsFRUSTUM(std::vector<GameObject*>& objects, const Frustum & primitive) const
{
	CollisionType out_col = primitive.ContainsBox(size);
		if (out_col != OUTSIDE)
		{
			for (std::list<GameObject*>::const_iterator it = this->elements.begin(); it != this->elements.end(); ++it)
			{
				CollisionType in_col = primitive.ContainsBox((*it)->aabb);
				if (in_col != OUTSIDE)
				{
						objects.push_back(*it);
					}
			}
			if (nodes[0] != nullptr) {
				for (int i = 0; i < SUBDIVISIONS; ++i)
					nodes[i]->CollectIntersections(objects, primitive);
			}
		}
}

#endif

