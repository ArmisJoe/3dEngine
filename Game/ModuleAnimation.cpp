#include "ModuleAnimation.h"
#include "Application.h"
#include "ResourceAnimation.h"
#include "ComponentTransform.h"
#include "ComponentBone.h"

ModuleAnimation::ModuleAnimation(bool start_enabled) : Module(start_enabled)
{
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

update_status ModuleAnimation::PreUpdate()
{
	return UPDATE_CONTINUE;
}

update_status ModuleAnimation::Update()
{
	for (vector<GameObject*>::iterator it = App->scene->GetRoot()->children.begin(); it != App->scene->GetRoot()->children.end(); ++it)
	{
		if ((*it) != nullptr)
		{
			ResetAllDeformableMeshes((*it));
			DeformMeshes(*it);
		}
	}


	return UPDATE_CONTINUE;
}

update_status ModuleAnimation::PostUpdate()
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
	for (vector<GameObject*>::iterator it = go->children.begin(); it != go->children.end(); ++it)
	{
		if ((*it) != nullptr)
		{
			// We should loop onto every bone
			// and call AdaptToBone((*it));
			vector<Component*> comps = go->FindComponents(componentType_Bone);
			if (!comps.empty())
			{
				for (uint i = 0; i < comps.size(); ++i)
				{
					AdaptToBone((ComponentBone*)comps[i]);
				}
			}

			DeformMeshes((*it));
		}
	}
}

void ModuleAnimation::AdaptToBone(ComponentBone * bone)
{
	if (bone != nullptr)
		return;

	/*ComponentMesh* m = bone->GetParent();
	if (m != nullptr)
		return;
			
	float4x4 trans = m->GetParent()->GetTransform()->GetGlobalTransformMatrix();
	trans = trans * m->GetParent()->GetTransform()->GetLocalTransformMatrix().Inverted();
	//trans = trans * rbone->offset; // OFFSET?¿?¿?¿?*/

}
