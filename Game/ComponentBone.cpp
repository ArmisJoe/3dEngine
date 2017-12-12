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
	for (uint i = 0; i < skeleton.size(); ++i)
	{
		float3 position, scale;
		float4x4 rot;
		skeleton[i]->offsetMat.Decompose(position, rot, scale);
		App->renderer3D->debugger->DrawAABB(position, scale);
	}
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
