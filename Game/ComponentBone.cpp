#include "ComponentBone.h"
#include "Application.h"

#include "imgui/imgui.h"
#include "mmgr\mmgr.h"

ComponentBone::ComponentBone() : Component(componentType_Bone)
{
	typeName = "Skeleton";
	name = "Bone";
}

ComponentBone::ComponentBone(GameObject * argparent) : Component(componentType_Bone, argparent)
{
	typeName = "Skeleton";
	name = "Bone";
}
 
ComponentBone::ComponentBone(componentType argtype, GameObject * argparent) : Component(componentType_Bone, argparent)
{
	typeName = "Skeleton";
	name = "Bone";
}

ComponentBone::~ComponentBone()
{
	typeName = "Skeleton";
}

void ComponentBone::Start()
{
}

void ComponentBone::Update(float dt)
{
	//for (uint i = 0; i < skeleton.size(); ++i)
	//{
	//	float3 position, scale;
	//	float4x4 rot;
	//	skeleton[i]->offsetMat.Decompose(position, rot, scale);
	//	App->renderer3D->debugger->DrawAABB(position, scale);
	//}
}

void ComponentBone::CleanUp()
{
	for (int i = 0; i < skeleton.size(); i++) {
		if (skeleton[i] != nullptr) {
			skeleton[i]->CleanUp();
			mdelete skeleton[i];
		}
	}
}

void ComponentBone::Enable()
{
	isActive = true;
}

void ComponentBone::Disable()
{
	isActive = false;
}

void ComponentBone::OnEditor()
{
	int num_bones = skeleton.size();
	ImGui::TextColored(COLOR_YELLOW, "Num Bones:");
	ImGui::InputInt("const", &num_bones);
	ImGui::TextColored(COLOR_YELLOW, "Bones:");
	if (ImGui::CollapsingHeader("Bones:")) {
		for (int i = 0; i < skeleton.size(); i++) {
			ImGui::TextColored(COLOR_YELLOW, "%i: ", i);
			ImGui::SameLine();
			ImGui::Text(skeleton[i]->name.c_str());
		}
	}
}

ComponentMesh * ComponentBone::GetMesh() const
{
	return mesh;
}

void ComponentBone::SetMesh(ComponentMesh * m)
{
	mesh = m;
}
void ComponentBone::MatchBoneAndGo(ResourceBone * bone, GameObject * go)
{

	go->FixAutoNaming();

	if (bone->name == go->GetName()) {
		bone->object = go;
		return;
	}

	for (int i = 0; i < go->children.size(); i++) {
		MatchBoneAndGo(bone, go->children[i]);
	}

}
void ComponentBone::CollectGOs(GameObject * go)
{
	PairedGOToBones tmp;
	tmp.object = go;
	tmp.name = go->GetName();
	pairedgotobones.push_back(tmp);

	for (uint i = 0; i < go->children.size(); ++i)
		CollectGOs(go->children[i]);
}
bool ComponentBone::insert_BoneToIterate(ResourceBone * bone)
{
	if (bone == nullptr || gos_filled == false)
		return false;

	for (uint i = 0; i < pairedgotobones.size(); ++i) {
		if (pairedgotobones[i].name == bone->name)
		{
			pairedgotobones[i].bones.push_back(bone);
			bone->object = pairedgotobones[i].object;
			return true;
		}
	}
	return false;
}
void ComponentBone::GetRootBone(GameObject* go)
{
	if (rootBoneGO != nullptr)
		return;
	for (uint i = 0; i < go->children.size(); ++i)
	{
		if (rootBoneGO != nullptr)
			return;
		for (uint j = 0; j < skeleton.size(); ++j)
		{
			if (rootBoneGO != nullptr)
				return;
			if (skeleton[j]->name == go->children[i]->GetName())
			{
				rootBoneGO = go->children[i];
				return;
			}
		}
		GetRootBone(go->children[i]);
	}

}

GameObject * ComponentBone::GetRootBoneGO()
{
	if (rootBoneGO != nullptr)
		return rootBoneGO;
	GetRootBone(App->scene->GetRoot());

	if (rootBoneGO != nullptr)
		return rootBoneGO;
	return nullptr;
}
void ComponentBone::GetGOFromBones()
{
	if (gos_filled == false)
	{
		CollectGOs(App->scene->GetRoot());
		gos_filled = true;
	}

	if (pairs_filled == false)
	{
		for (uint i = 0; i < skeleton.size(); ++i)
		{
			insert_BoneToIterate(skeleton[i]);
		}

		/*for (vector<PairedGOToBones>::iterator it = pairedgotobones.begin(); it != pairedgotobones.end();)
		{
			if ((*it).bones.empty() == true)
				it = pairedgotobones.erase(it);

			++it;
		}*/
		pairs_filled = true;
	}
}

void ComponentBone::LinkBonesToGO()
{
	for (int i = 0; i < skeleton.size(); i++) {
		if(skeleton[i]->object != nullptr)
			continue;
		MatchBoneAndGo(skeleton[i], App->scene->GetRoot());
	}
}

