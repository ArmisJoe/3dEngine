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
	AABB aabb;
	aabb.SetFromCenterAndSize(float3(0, 0, 0), float3(-500, 100, 500));
	quadtreeh->SetMaxSize(aabb);

	return true;
}

update_status ModuleQuadtree::Update(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_P) == KEY_UP) quadtreeh->root->Devide();

	vector<AABB> AABBvector;

	quadtreeh->CollectAllNodes(AABBvector);

	for (uint i = 0; i < AABBvector.size(); ++i)
	{
		App->renderer3D->debugger->DrawAABB(AABBvector[i].CenterPoint(), AABBvector[i].Size());
	}

	/*if (App->input->GetKey(SDL_SCANCODE_1) == KEY_UP)
	{
		App->scene->GetRoot()->children[0]->SetStatic(true);
		quadtreeh->Insert(App->scene->GetRoot()->children[0]);
	}*/


	AABBvector.clear();

	return UPDATE_CONTINUE;
}

bool ModuleQuadtree::CleanUp()
{
	quadtreeh->Clear();
	delete quadtreeh;

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

void ModuleQuadtree::RestartQuadtree()
{
	CleanUp();
	quadtreeh = new Quadtree();
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
