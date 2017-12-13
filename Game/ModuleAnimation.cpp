#include "ModuleAnimation.h"
#include "Application.h"
#include "ResourceAnimation.h"
#include "ComponentTransform.h"
#include "ComponentBone.h"

ModuleAnimation::ModuleAnimation(bool start_enabled) : Module(start_enabled)
{
	name = "Animation";
}


ModuleAnimation::~ModuleAnimation()
{
}

bool ModuleAnimation::Init()
{
	return true;
}

bool ModuleAnimation::Start()
{
	return true;
}

update_status ModuleAnimation::PreUpdate(float dt)
{
	if (App->scene->GetRoot() != nullptr) {
		//ResetAllDeformableMeshes(App->scene->GetRoot());
		//DeformMeshes(App->scene->GetRoot());
	}
	return UPDATE_CONTINUE;
}

update_status ModuleAnimation::Update(float dt)
{


	//for (vector<GameObject*>::iterator it = App->scene->GetRoot()->children.begin(); it != App->scene->GetRoot()->children.end(); ++it)
	//{
	//	if ((*it) != nullptr)
	//	{
	//		ResetAllDeformableMeshes((*it));
	//		DeformMeshes(*it);
	//	}
	//}


	return UPDATE_CONTINUE;
}

update_status ModuleAnimation::PostUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

bool ModuleAnimation::CleanUp()
{
	return true;
}

void ModuleAnimation::ResetAllDeformableMeshes(GameObject* go)
{
	for (vector<GameObject*>::iterator it = go->children.begin(); it != go->children.end(); ++it)
	{
		if ((*it) != nullptr)
		{
			vector<Component*> comps = go->FindComponents(componentType_Mesh);
			if (!comps.empty())
			{
				for (uint i = 0; i < comps.size(); ++i)
				{
					ComponentMesh* m = (ComponentMesh*)comps[i];
					m->ResetDeformableMesh();
				}
			}
			ResetAllDeformableMeshes((*it));
		}
	}
}

void ModuleAnimation::DeformMeshes(GameObject* go)
{
	if (go == nullptr)
		return;
	// We should loop onto every bone
	// and call AdaptToBone((*it));
	ComponentBone* skeleton = (ComponentBone*)go->FindComponent(componentType_Bone);
	if (skeleton != nullptr) {
		AdaptToBone(skeleton);
	}
	if(!go->children.empty())
	for (int i = 0; i < go->children.size(); i++)
	{
		GameObject* it = go->children[i];
		if ( it != nullptr)
		{
			DeformMeshes(it);
		}
	}
}

void ModuleAnimation::AdaptToBone(ComponentBone * skeleton)
{
	if (skeleton == nullptr || skeleton->GetMesh() == nullptr)
		return;
	
	skeleton->GetMesh()->ResetDeformableMesh(); // Skin Reset

	ComponentMesh* deformable = skeleton->GetMesh()->skin;

	for (int i = 0; i < skeleton->skeleton.size(); i++) {
		ResourceBone* b = skeleton->skeleton[i];
		GameObject* boneGO = nullptr;
		skeleton->GetGOFromBones();
		boneGO = b->object;
		//boneGO = CheckGoBoneMatch(App->scene->GetRoot(), b);
		if (boneGO == nullptr)
			return;

		float4x4 trans = boneGO->GetTransform()->GetGlobalTransformMatrix(); // Gets the boneGO trans
		trans = trans * skeleton->GetParent()->GetTransform()->GetLocalTransformMatrix().Inverted(); // Applies Mesh Transformations
		trans = trans * b->offsetMat; // Applies offset

		for (int k = 0; k < b->num_weigths; k++) {
			uint idx = b->indices[i];
			float3 originalV(&skeleton->GetMesh()->vertices[idx * 3]);
			float3 addV = trans.TransformPos(originalV);
			deformable->vertices[idx * 3] += addV.x * b->weigths[i];
			deformable->vertices[idx * 3 + 1] += addV.y * b->weigths[i];
			deformable->vertices[idx * 3 + 2] += addV.z * b->weigths[i];
		}

	}

	skeleton->GetMesh()->BindSkin();  // Skin Bind

}

void ModuleAnimation::AdaptMeshToBone(ComponentBone * skeleton, ComponentMesh * mesh)
{
	if (skeleton == nullptr || mesh == nullptr)
		return;

	mesh->ResetDeformableMesh(); // Skin Reset

	ComponentMesh* deformable = mesh->skin;

	for (int i = 0; i < skeleton->skeleton.size(); i++) {
		ResourceBone* b = skeleton->skeleton[i];
		GameObject* boneGO = nullptr;
		skeleton->GetGOFromBones();
		boneGO = b->object;

		//boneGO = CheckGoBoneMatch(App->scene->GetRoot(), b);
		if (boneGO == nullptr)
			return;

		float4x4 trans = boneGO->GetTransform()->GetGlobalTransformMatrix(); // Gets the boneGO trans
		trans = trans * skeleton->GetParent()->GetTransform()->GetLocalTransformMatrix().Inverted(); // Applies Mesh Transformations
		trans = trans * b->offsetMat; // Applies offset

		for (int k = 0; k < b->num_weigths; k++) {
			uint idx = b->indices[k];
			float3 originalV(&mesh->vertices[idx * 3]);
			float3 addV = trans.TransformPos(originalV);
			deformable->vertices[idx * 3] += addV.x * b->weigths[k];
			deformable->vertices[idx * 3 + 1] += addV.y * b->weigths[k];
			deformable->vertices[idx * 3 + 2] += addV.z * b->weigths[k];
		}

	}

	mesh->BindSkin();  // Skin Bind

}

GameObject * ModuleAnimation::CheckGoBoneMatch(GameObject * go, ResourceBone * b)
{
	GameObject* ret = nullptr;

	if (go == nullptr || b == nullptr)
		return nullptr;

	if (go->GetName() == b->name)
		return go;

	for (int i = 0; i < go->children.size(); i++) {
		ret = CheckGoBoneMatch(go->children[i], b);
		if (ret != nullptr)
			break;
	}

	return ret;
}
