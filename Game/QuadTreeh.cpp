#include "QuadTreeh.h"
#include "Globals.h"

QuadtreeNode::QuadtreeNode()
{
	parent = nullptr;
	nodes[0] = nullptr; nodes[1] = nullptr; nodes[2] = nullptr; nodes[3] = nullptr;
}
QuadtreeNode::QuadtreeNode(const AABB & limits){
	size = limits;
	parent = nullptr;
	nodes[0] = nullptr; nodes[1] = nullptr; nodes[2] = nullptr; nodes[3] = nullptr;
}

QuadtreeNode::QuadtreeNode(const AABB &aabb, QuadtreeNode* node)
{
	if (node != nullptr) {
		parent = node;
	}

	nodes[0] = nullptr; nodes[1] = nullptr; nodes[2] = nullptr; nodes[3] = nullptr;

	size = aabb;
}

QuadtreeNode::~QuadtreeNode()
{
	for (int i = 0; i < SUBDIVISIONS; ++i)
	{
		if (nodes[i] != nullptr)
		{
			delete(nodes[i]);
			// I really hope this does not crash
			// nvm it does lol
		}
	}
}

void QuadtreeNode::Insert(GameObject * go)
{
	//When we insert a new item in the quadtree:
	//is it to the root node
	if (nodes[0] == nullptr)
	{
		//If it is within the limits of the Quadtree
		if (elements.size() < MAX_ELEMENTS || (size.HalfSize().LengthSq() <= MIN_SIZE * MIN_SIZE))
			elements.push_back(go);

		else
		{
			//If you run out of Bucket space and you are a leaf subdivide in 4 (where the magic happens)
			Devide();
			//Add it node’s list
			elements.push_back(go);
			//Redistribute all GameObjects to proper childs based on their position in space
			Redistribute();
		}
	}
	else
	{
		//Add it node’s list
		elements.push_back(go);
		//Redistribute all GameObjects to proper childs based on their position in space
		Redistribute();
	}
}

void QuadtreeNode::Remove(GameObject * go)
{
	std::list<GameObject*>::iterator it = std::find(elements.begin(), elements.end(), go);
	if (it != elements.end())
		elements.erase(it);

	bool Leaf = (nodes[0] == nullptr);

	if (Leaf == false)
	{
		for (int i = 0; i < 4; ++i)
			nodes[i]->Remove(go);
	}

}


void QuadtreeNode::Create(const AABB & limits)
{
 // why does the handout say we need this?	
}

void QuadtreeNode::Redistribute()
{
	// In my head it makes sense

	for (std::list<GameObject*>::iterator it = elements.begin(); it != elements.end();)
	{
		AABB new_box((*it)->aabb);

		bool intersect = true;
		for (int i = 0; i < SUBDIVISIONS; ++i)
		{
			if (nodes[i]->size.Intersects(new_box))
				continue;
			else {
				intersect = false;
				break;
			}
		}
		if (intersect == true)
			++it;
		else
		{
			// if it doesn't intersect with all nodes we put it in the ones that does
			it = elements.erase(it);
			for (int i = 0; i < SUBDIVISIONS; ++i) {
				if (nodes[i]->size.Intersects(new_box))
					nodes[i]->Insert(*it);
			}
		}
	}
}

void QuadtreeNode::Devide()
{
	// We need to subdivide this node ...
	float3 size(this->size.Size());
	float3 new_size(size.x*0.5f, size.y, size.z*0.5f); // Octree would subdivide y too

	float3 center(this->size.CenterPoint());
	float3 new_center(center);
	AABB new_box;

	// NorthEast
	new_center.x = center.x + size.x * 0.25f;
	new_center.z = center.z + size.z * 0.25f;
	new_box.SetFromCenterAndSize(new_center, new_size);
	nodes[0] = new QuadtreeNode(new_box);

	// SouthEast
	new_center.x = center.x + size.x * 0.25f;
	new_center.z = center.z - size.z * 0.25f;
	new_box.SetFromCenterAndSize(new_center, new_size);
	nodes[1] = new QuadtreeNode(new_box);

	// SouthWest
	new_center.x = center.x - size.x * 0.25f;
	new_center.z = center.z - size.z * 0.25f;
	new_box.SetFromCenterAndSize(new_center, new_size);
	nodes[2] = new QuadtreeNode(new_box);

	// NorthWest
	new_center.x = center.x - size.x * 0.25f;
	new_center.z = center.z + size.z * 0.25f;
	new_box.SetFromCenterAndSize(new_center, new_size);
	nodes[3] = new QuadtreeNode(new_box);
}

template<typename TYPE>
inline void QuadtreeNode::CollectIntersections(std::vector<GameObject*>& objects, const TYPE & primitive) const
{
	if (primitive.Intersects(box))
	{
		for (std::list<GameObject*>::const_iterator it = this->objects.begin(); it != this->objects.end(); ++it)
		{
			if (primitive.Intersects((*it)->global_bbox))
				objects.push_back(*it);

		}
		for (int i = 0; i < 4; ++i)
			if (childs[i] != nullptr) childs[i]->CollectIntersections(objects, primitive);

	}
}


// --------------------------------------------------------------------
//							QUADTREE
//---------------------------------------------------------------------

Quadtree::Quadtree()
{
}

Quadtree::~Quadtree()
{
	if (root != nullptr)
	{
		delete root;
	}
}

void Quadtree::SetMaxSize(const AABB & box)
{
	Clear();
	root = new QuadtreeNode(box);
}

void Quadtree::Insert(GameObject * go)
{
	if (root != nullptr)
	{
		string ska(go->aabb.ToString());
		string skiski(root->GetBox().ToString());

		if (root->GetBox().Intersects(go->aabb))
			root->Insert(go);
	}
}

void Quadtree::Erase(GameObject * go)
{
	if (root != nullptr)
		root->Remove(go);
}

void Quadtree::Clear()
{
	//tecnhically de destructor will handle the inside data...
}


void Quadtree::Divide()
{
}

void Quadtree::Intersections(std::vector<GameObject*>& objects, const AABB & aabb) const
{
}

void Quadtree::CollectAllNodes(vector<AABB> &nodes)
{
	if (root != nullptr)
	{
		RecursiveNodes(nodes, root);
		nodes.push_back(root->size);
	}
}

void Quadtree::RecursiveNodes(vector<AABB> &nodes, QuadtreeNode * it)
{
	for (uint i = 0; i < SUBDIVISIONS; ++i)
	{
		if (it->nodes[i] != nullptr)
		{
			RecursiveNodes(nodes, it->nodes[i]);
			nodes.push_back(it->nodes[i]->size);
		}
	}
}

void Quadtree::ClearNode(QuadtreeNode * point)
{
	for (int i = 0; i < SUBDIVISIONS; ++i)
	{
		if (point->nodes[i] != nullptr)
		{
			ClearNode(point->nodes[i]);
			delete(point->nodes[i]);
			// I really hope this does not crash
			// nvm it does lol
		}
	}
}

