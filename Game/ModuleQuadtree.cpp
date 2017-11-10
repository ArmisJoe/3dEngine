//#include "ModuleRenderer3D.h"
#include "ModuleQuadtree.h"
#include "Application.h"

ModuleQuadtree::ModuleQuadtree(bool start_enabled) : Module(start_enabled)
{
	name = "quadTree";
}

ModuleQuadtree::~ModuleQuadtree()
{
}

bool ModuleQuadtree::Start()
{
	AABB aabb;
	aabb.SetFromCenterAndSize(float3(0, 0, 0), float3(20, 20, 30));
	quadtreeh.SetMaxSize(aabb);

	return true;
}

update_status ModuleQuadtree::Update(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_P) == KEY_UP) quadtreeh.root->Devide();

	vector<AABB> AABBvector;

	quadtreeh.CollectAllNodes(AABBvector);

	for (uint i = 0; i < AABBvector.size(); ++i)
	{
		App->renderer3D->debugger->DrawAABB(AABBvector[i].CenterPoint(), AABBvector[i].Size());
	}

	AABBvector.clear();

	return UPDATE_CONTINUE;
}

bool ModuleQuadtree::CleanUp()
{
	return true;
}
