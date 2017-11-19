#include "QuadTreeh.h"
#include "Globals.h"
#include "Application.h"

QuadtreeNode::QuadtreeNode()
{
	parent = nullptr;
	nodes[0] = nullptr; nodes[1] = nullptr; nodes[2] = nullptr; nodes[3] = nullptr;
	nodes[4] = nullptr; nodes[5] = nullptr; nodes[6] = nullptr; nodes[7] = nullptr;
}
QuadtreeNode::QuadtreeNode(const AABB & limits){
	size = limits;
	parent = nullptr;
	nodes[0] = nullptr; nodes[1] = nullptr; nodes[2] = nullptr; nodes[3] = nullptr;
	nodes[4] = nullptr; nodes[5] = nullptr; nodes[6] = nullptr; nodes[7] = nullptr;
}

QuadtreeNode::QuadtreeNode(const AABB &aabb, QuadtreeNode* node)
{
	if (node != nullptr) {
		parent = node;
	}

	nodes[0] = nullptr; nodes[1] = nullptr; nodes[2] = nullptr; nodes[3] = nullptr;
	nodes[4] = nullptr; nodes[5] = nullptr; nodes[6] = nullptr; nodes[7] = nullptr;

	size = aabb;
}

QuadtreeNode::~QuadtreeNode()
{
	for (int i = 0; i < SUBDIVISIONS; ++i)
	{
		if (nodes[i] != nullptr)
		{
			delete(nodes[i]);
			nodes[i] = nullptr;
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
		if (elements.size() < MAX_ELEMENTS /*|| (size.HalfSize().LengthSq() <= MIN_SIZE * MIN_SIZE)*/)
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
		for (int i = 0; i < SUBDIVISIONS; ++i)
			nodes[i]->Remove(go);
	}

}


void QuadtreeNode::Create(const AABB & limits)
{
 // why does the handout say we need this?	
}

void QuadtreeNode::Redistribute()
{

	for (std::list<GameObject*>::iterator it = elements.begin(); it != elements.end();)
	{
		if ((*it) != nullptr && (*it)->HasAABB == true)
		{
			GameObject* go = (*it);
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
						nodes[i]->Insert(go); //  while the chimney starts subdiving like crazy
				}
			}
		}
		else
		{
			it = elements.erase(it);
		}
	}
}

void QuadtreeNode::Devide()
{
	AABB box;
	float3 childs_side_length = size.Size() / 2;
	float3 center = size.CenterPoint();

	int child_index = 0;
	for (int x = 0; x < 2; x++)
	{
		for (int y = 0; y < 2; y++)
		{
			for (int z = 0; z < 2; z++)
			{
				float3 min_child_point(
					size.minPoint.x + z * childs_side_length.x,
					size.minPoint.y + y * childs_side_length.y,
					size.minPoint.z + x * childs_side_length.z
				);

				float3 max_child_point(
					min_child_point.x + childs_side_length.x,
					min_child_point.y + childs_side_length.y,
					min_child_point.z + childs_side_length.z
				);
				box.minPoint = min_child_point;
				box.maxPoint = max_child_point;
				nodes[child_index] = new QuadtreeNode(box);
				child_index++;
			}
		}
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
		root = nullptr;
	}
}

void Quadtree::SetMaxSize(const AABB & box)
{
	Clear();

	if (root != nullptr)
		delete root;
	root = new QuadtreeNode(box);

}

void Quadtree::Insert(GameObject * go)
{
	if (root != nullptr && go != nullptr && go->HasAABB == true)
	{
			if (Adapt(go->aabb)) {
				AABB nn_aabb(root->size);
				delete root;
				root = nullptr;
				root = new QuadtreeNode(nn_aabb);
				root->Insert(go);
				if (!App->scene->static_gos.empty())
				{
					for (uint i = 0; i < App->scene->static_gos.size(); ++i)
					{
						if (App->scene->static_gos[i] != nullptr)
							root->Insert(App->scene->static_gos[i]);
					}
				}
				App->scene->static_gos.push_back(go);
			}
			else {
				if (root->GetBox().Intersects(go->aabb))
				{
					root->Insert(go);
					App->scene->static_gos.push_back(go);
				}
			}
		}
}

void Quadtree::Erase(GameObject * go)
{
	if (root != nullptr)
		root->Remove(go);
}

void Quadtree::Clear()
{
	if (root != nullptr) {
		delete root;
		root = nullptr;
	}
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
			point->nodes[i] = nullptr;
			// I really hope this does not crash
			// nvm it does lol
		}
	}
}

bool Quadtree::Adapt(const AABB &aabb)
{
	bool adapted = false;

	if (aabb.minPoint.x < root->size.minPoint.x)
	{
		root->size.minPoint.x = aabb.minPoint.x;
		adapted = true;
	}
	if (aabb.minPoint.y < root->size.minPoint.y)
	{
		root->size.minPoint.y = aabb.minPoint.y;
		adapted = true;
	}
	if (aabb.minPoint.z < root->size.minPoint.z)
	{
		root->size.minPoint.z = aabb.minPoint.z;
		adapted = true;
	}
	if (aabb.maxPoint.x > root->size.maxPoint.x)
	{
		root->size.maxPoint.x = aabb.maxPoint.x;
		adapted = true;
	}
	if (aabb.maxPoint.y > root->size.maxPoint.y)
	{
		root->size.maxPoint.y = aabb.maxPoint.y;
		adapted = true;
	}
	if (aabb.maxPoint.z > root->size.maxPoint.z)
	{
		root->size.maxPoint.z = aabb.maxPoint.z;
		adapted = true;
	}
	return adapted;
}

