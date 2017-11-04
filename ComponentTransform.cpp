#include "ComponentTransform.h"

#include "ImGui\imgui.h"

ComponentTransform::ComponentTransform() : Component(componentType_Transform)
{
	name = "Transform";
	numMax = 1;
	position = { 0, 0, 0 };
	rotation = { 0, 0, 0, 0 };
	scale = { 1, 1, 1 };
}

ComponentTransform::ComponentTransform(GameObject* argparent) : Component(componentType_Transform, argparent)
{
	name = "Transform";
	numMax = 1;
	position = { 0, 0, 0 };
	rotation = { 0, 0, 0, 0 };
	scale = { 1, 1, 1 };
}

ComponentTransform::ComponentTransform(componentType argtype, GameObject * argparent) : Component(componentType_Transform, argparent)
{
	name = "Transform";
	numMax = 1;
	position = { 0, 0, 0 };
	rotation = { 0, 0, 0, 0 };
	scale = { 1, 1, 1 };
}


void ComponentTransform::UpdateMatrix() {	
	WorldMatrix = float4x4::FromTRS(position, rotation, scale);
	for (std::vector<GameObject*>::iterator it = GetParent()->children.begin(); it != GetParent()->children.end(); it++)
	{
		ComponentTransform* child_transform = (ComponentTransform*)(*it)->FindComponents(componentType_Transform)[0];
		child_transform->UpdateMatrix();
	}
}

void ComponentTransform::OnEditor()
{
	float pos[3];
	float rot[4];
	float sca[3];

	pos[0] = position.x;
	pos[1] = position.y;
	pos[2] = position.z;

	rot[0] = rotation.x;
	rot[1] = rotation.y;
	rot[2] = rotation.z;
	rot[3] = rotation.w;

	sca[0] = scale.x;
	sca[1] = scale.y;
	sca[2] = scale.z;

	ImGui::InputFloat3("Position: ", pos);
	ImGui::InputFloat3("Rotation: ", rot);
	ImGui::InputFloat3("Scale: ", sca);

	position.x = pos[0];
	position.y = pos[1];
	position.z = pos[2];

	rotation.x = rot[0];
	rotation.y = rot[1];
	rotation.z = rot[2];
	rotation.w = rot[3];

	scale.x = sca[0];
	scale.y = sca[1];
	scale.z = sca[2];
}
