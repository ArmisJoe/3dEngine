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

ComponentTransform::ComponentTransform(GameObject * argparent, float3 position, Quat rotation, float3 scale) : Component(componentType_Transform, argparent)
{
	this->position = position; this->rotation = rotation; this->scale = scale;
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
	float rot[3];
	float sca[3];

	pos[0] = position.x;
	pos[1] = position.y;
	pos[2] = position.z;

	rot[0] = RadToDeg(rotation.ToEulerXYZ().x);
	rot[1] = RadToDeg(rotation.ToEulerXYZ().y);
	rot[2] = RadToDeg(rotation.ToEulerXYZ().z);

	sca[0] = scale.x;
	sca[1] = scale.y;
	sca[2] = scale.z;

	if (ImGui::DragFloat3("Position:", pos, 0.1f)) { 
		position.x = pos[0];
		position.y = pos[1];
		position.z = pos[2];
	}
	if (ImGui::DragFloat3("Rotation:", rot, 0.1f)) {
		rotation = rotation.FromEulerXYZ(DegToRad(rot[0]), DegToRad(rot[1]), DegToRad(rot[2]));
	}
	
	if (ImGui::DragFloat3("Scale:", sca, 0.1f)) {
		scale.x = sca[0];
		scale.y = sca[1];
		scale.z = sca[2];
	}

}

float* ComponentTransform::GetMatrix4x4() const
{
	const float4x4 retMat = WorldMatrix.FromTRS(position, rotation, scale);

	return retMat.Transposed().ptr();
}
