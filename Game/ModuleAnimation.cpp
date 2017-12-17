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
	return UPDATE_CONTINUE;
}

update_status ModuleSkinning::Update(float dt)
{
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


void ModuleSkinning::AdaptMeshToBone(ComponentBone * skeleton, ComponentMesh * mesh)
{

	if (mesh == nullptr || skeleton == nullptr)
		return;

	mesh->ResetDeformableMesh(); // Skin Reset

	for (int i = 0; i < skeleton->skeleton.size(); i++) {
		// calc the transformation of this bone based on its root (not the global transformation)
		ResourceBone* b = skeleton->skeleton[i];
		if (b->object == nullptr)
			skeleton->LinkBonesToGO();
		if (b->object == nullptr)
			continue;

		float4x4 trans = b->object->GetTransform()->GetGlobalTransform();
		trans = trans * mesh->GetParent()->GetTransform()->GetTransform().Inverted();

		// Now apply a transformation to place the vertex as it was in the bind pose
		trans = trans * b->offsetMat;
		ComponentMesh* deformable = mesh->skin;

		for (uint n = 0; n < b->num_weigths; ++n)
		{
			uint index = b->indices[n];
			float3 original(&mesh->vertices[index * 3]);
			float3 vertex(&deformable->vertices[index * 3]);

			if (deformable->indices[index]++ == 0)
			{
				memset(&deformable->vertices[index * 3], 0, sizeof(float) * 3);
				if (mesh->normals)
					memset(&deformable->normals[index * 3], 0, sizeof(float) * 3);
			}

			vertex = trans.TransformPos(original);

			deformable->vertices[index * 3] += vertex.x * b->weigths[n];
			deformable->vertices[index * 3 + 1] += vertex.y * b->weigths[n];
			deformable->vertices[index * 3 + 2] += vertex.z * b->weigths[n];

			//App->renderer3D->debugger->DrawAABB(vertex, float3(0.05, 0.05, 0.05), float3(255, 0, 0));

			if (mesh->normals)
			{
				vertex = trans.TransformPos(float3(&mesh->normals[index * 3]));
				deformable->normals[index * 3] += vertex.x * b->weigths[n];
				deformable->normals[index * 3 + 1] += vertex.y * b->weigths[n];
				deformable->normals[index * 3 + 2] += vertex.z * b->weigths[n];
			}
		}

	}
	int b = 0;
	for (int i = 0; i < mesh->skin->num_vertices; i++) {
		float3 V;
		V.x = mesh->skin->vertices[i * 3];
		V.y = mesh->skin->vertices[i * 3 + 1];
		V.z = mesh->skin->vertices[i * 3 + 2];
		int kaka = 0;
	}

	mesh->BindSkin();
}

/*
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
		GameObject* sk = skeleton->GetRootBoneGO();

		float4x4 mat = b->object->GetTransform()->GetGlobalTransform();
		mat = skeleton->GetRootBoneGO()->GetParent()->GetParent()->GetTransform()->GetTransform().Inverted() * mat;
		mat = mesh->GetParent()->GetTransform()->GetTransform().Inverted() * mat;
		mat = mat * b->offsetMat;

		for (int i = 0; i < b->num_weigths; i++) {
			uint index = b->indices[i];
			float3 originalV(&mesh->vertices[index * 3]);

			float3 addV = mat.TransformPos(originalV);

			deformable->vertices[index * 3] += addV.x * b->weigths[i];
			deformable->vertices[index * 3 + 1] += addV.y * b->weigths[i];
			deformable->vertices[index * 3 + 2] += addV.z * b->weigths[i];

			// Normals
			if (mesh->num_normals > 0) {
				addV = mat.TransformPos(float3(&mesh->normals[index * 3]));
				deformable->normals[index * 3] += addV.x * b->weigths[i];
				deformable->normals[index * 3 + 1] += addV.y * b->weigths[i];
				deformable->normals[index * 3 + 2] += addV.z * b->weigths[i];
			}
		}

		for (int n = 0; n < deformable->num_vertices; n++) {
			float* deformV = &deformable->vertices[n * 3];
			float* meshV = &mesh->vertices[n * 3];
			for (int idx = 0; idx < 3; idx++) {
				if (Abs(deformV[idx] - meshV[idx]) > 100) {
					deformV[idx] = meshV[idx];
				}
			}
			
		}
	}

	mesh->BindSkin();  // Skin Bind

}*/