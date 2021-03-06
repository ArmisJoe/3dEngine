#pragma once

#include "Module.h"
#include "Globals.h"
#include "glmath.h"
#include "GameObject.h"
#include "MathGeoLib/MathGeoLib.h"
#include "MathGeoLib/Geometry/QuadTree.h"
#include "QuadTreeh.h"

class ModuleQuadtree : public Module
{
	friend class ModuleEditorUI;
public:
	ModuleQuadtree(bool start_enabled = true);
	~ModuleQuadtree();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();
	bool InsertObject(GameObject* go); // Returns false if the object is not static
	void RemoveObject(GameObject*go);
	void RestartQuadtree();
	void ResetQuadtree(AABB aabb);
	bool GetRoot(AABB* getter) const; // returns false if root is nullptr
	template<typename TYPE>
	void CollectIntersections(std::vector<GameObject*>& objects, const TYPE & primitive) const;

private:
	Quadtree* quadtreeh; // I deadass created the quadtree class when there is one in mathgeolib ok
protected:
	bool visible = false;
};

template<typename TYPE>
inline void ModuleQuadtree::CollectIntersections(std::vector<GameObject*>& objects, const TYPE & primitive) const
{
	quadtreeh->root->CollectIntersections(objects, primitive);
}
