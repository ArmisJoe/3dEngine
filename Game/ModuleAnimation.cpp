#include "ModuleAnimation.h"
#include "Application.h"
#include "ResourceAnimation.h"
#include "ComponentTransform.h"
#include "ComponentBone.h"

ModuleSkinning::ModuleSkinning(bool start_enabled) : Module(start_enabled)
{
	name = "Animation";
}


ModuleSkinning::~ModuleSkinning()
{
}

bool ModuleSkinning::Init()
{
	return true;
}

bool ModuleSkinning::Start()
{
	return true;
}

update_status ModuleSkinning::PreUpdate(float dt)
{
	if (App->scene->GetRoot() != nullptr) {
		//ResetAllDeformableMeshes(App->scene->GetRoot());
		//DeformMeshes(App->scene->GetRoot());
	}
	return UPDATE_CONTINUE;
}

update_status ModuleSkinning::Update(float dt)
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

update_status ModuleSkinning::PostUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

bool ModuleSkinning::CleanUp()
{
	return true;
}

void ModuleSkinning::ResetAllDeformableMeshes(GameObject* go)
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

void ModuleSkinning::DeformMeshes(GameObject* go)
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

void ModuleSkinning::AdaptToBone(ComponentBone * skeleton)
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

		float4x4 trans = skeleton->GetRootBoneGO()->GetTransform()->GetGlobalTransform(); // Gets the boneGO trans
		trans = trans * skeleton->GetParent()->GetTransform()->GetTransform().Inverted(); // Applies Mesh Transformations
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

void ModuleSkinning::AdaptMeshToBone(ComponentBone * skeleton, ComponentMesh * mesh)
{
	if (skeleton == nullptr || mesh == nullptr)
		return;

	mesh->ResetDeformableMesh(); // Skin Reset

	ComponentMesh* deformable = mesh->skin;

	for (int i = 0; i < skeleton->skeleton.size(); i++) {
		ResourceBone* b = skeleton->skeleton[i];
		if (b->object == nullptr)
			skeleton->GetGOFromBones();
		if (b->object == nullptr)
			return;

		float4x4 mat = App->scene->GetRoot()->GetTransform()->GetGlobalTransform().Inverted() * b->object->GetTransform()->GetGlobalTransform();
		mat = mesh->GetParent()->GetTransform()->GetTransform().Inverted() * mat;
		mat = mat * b->offsetMat;

		for (int n = 0; n < b->num_weigths; n++) {
			uint index = b->indices[n];
			float3 originalV(&mesh->vertices[index * 3]);

			float3 addV = mat.TransformPos(originalV);

			deformable->vertices[index * 3] += addV.x * b->weigths[n];
			deformable->vertices[index * 3 + 1] += addV.y * b->weigths[n];
			deformable->vertices[index * 3 + 2] += addV.z * b->weigths[n];

			// Normals
			if (mesh->num_normals > 0) {
				addV = mat.TransformPos(float3(&mesh->normals[index * 3]));
				deformable->normals[index * 3] += addV.x * b->weigths[n];
				deformable->normals[index * 3 + 1] += addV.y * b->weigths[n];
				deformable->normals[index * 3 + 2] += addV.z * b->weigths[n];
			}
		}

		for (int n = 0; n < deformable->num_vertices; n++) {
			float* deformV = &deformable->vertices[n * 3];
			float* meshV = &mesh->vertices[n * 3];
			for (int idx = 0; idx < 3; idx++) {
				if (Abs(deformV[idx] - meshV[idx]) > 10) {
					deformV[idx] = meshV[idx];
				}
			}
			
		}

		//for (int k = 0; k < b->num_weigths; k++) {
		//	uint idx = b->indices[k];
		//	float3 originalV(&mesh->vertices[idx * 3]);
		//	float3 addV = trans.TransformPos(originalV);
		//	deformable->vertices[idx * 3] += addV.x * b->weigths[k];
		//	deformable->vertices[idx * 3 + 1] += addV.y * b->weigths[k];
		//	deformable->vertices[idx * 3 + 2] += addV.z * b->weigths[k];
		//}

	}

	mesh->BindSkin();  // Skin Bind

}

GameObject * ModuleSkinning::CheckGoBoneMatch(GameObject * go, ResourceBone * b)
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
