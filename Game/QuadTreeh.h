#pragma once

#include "GameObject.h"
#include "MathGeoLib/MathGeoLib.h"
#include <list>
#include <map>

#define MAX_ELEMENTS 8
#define SUBDIVISIONS 4 
#define MIN_SIZE 5.0f 
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
	void CollectIntersections(std::vector<GameObject*>& objects, const TYPE& primitive) const;
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

public:
	QuadtreeNode* root = nullptr;
	uint max_divisions = DEPTH;
};