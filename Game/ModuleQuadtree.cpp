//#include "ModuleRenderer3D.h"
#include "ModuleQuadtree.h"
#include "Application.h"


ModuleQuadtree::ModuleQuadtree(bool start_enabled) : Module(start_enabled)
{
	name = "quadTree";
	quadtreeh = new Quadtree();
}

ModuleQuadtree::~ModuleQuadtree()
{
}

bool ModuleQuadtree::Start()
{

	quadtreeh->SetMaxSize(AABB(float3(-50, 0, -50), float3(50, 30, 50)));

	return true;
}

update_status ModuleQuadtree::Update(float dt)
{

	vector<AABB> AABBvector;
	
	quadtreeh->CollectAllNodes(AABBvector);
	
	for (uint i = 0; i < AABBvector.size(); ++i)
	{
		App->renderer3D->debugger->DrawAABB(AABBvector[i].CenterPoint(), AABBvector[i].Size());
	}
	
	std::vector< GameObject*> objects;
	quadtreeh->root->CollectIntersectionsFRUSTUM(objects, App->camera->curr_camera->GetFrustum());
	
	// end we delete duplicates
	sort(objects.begin(), objects.end());
	objects.erase(unique(objects.begin(), objects.end()), objects.end());
	
	for (uint i = 0; i < objects.size(); ++i)
	{
		App->renderer3D->AddGameObjectToDraw(objects[i]);
	}

	return UPDATE_CONTINUE;
}

bool ModuleQuadtree::CleanUp()
{
	quadtreeh->Clear();
	mdelete quadtreeh;

	return true;
}

bool ModuleQuadtree::InsertObject(GameObject * go)
{
	bool ret = true;
	if (!go->IsStatic())
		ret = false;

	if (ret != false)
	{
		quadtreeh->Insert(go);
	}
	return ret;
}

void ModuleQuadtree::RemoveObject(GameObject * go)
{
	vector<GameObject*> obj;
	
	for (uint i = 0; i < quadtreeh->static_gos.size(); ++i)
	{
		if (quadtreeh->static_gos[i]->GetName() != go->GetName())
		{
			obj.push_back(quadtreeh->static_gos[i]);
		}
	}
	
	RestartQuadtree();

	for (uint i = 0; i < obj.size(); ++i)
	{
		if (obj[i]->IsStatic())
		{
			InsertObject(obj[i]);
		}
	}
}

void ModuleQuadtree::RestartQuadtree()
{
	CleanUp();
	quadtreeh = new Quadtree();
	quadtreeh->SetMaxSize(AABB(float3(-50, 0, -50), float3(50, 30, 50)));
	quadtreeh->static_gos.clear();

}

void ModuleQuadtree::ResetQuadtree(AABB aabb)
{
	RestartQuadtree();
	quadtreeh->SetMaxSize(aabb);
}

bool ModuleQuadtree::GetRoot(AABB* getter) const
{
	getter = &quadtreeh->root->size;		
	return (quadtreeh->root != nullptr);
}
