#include "ComponentTransform.h"

#include "ImGui\imgui.h"

ComponentTransform::ComponentTransform() : Component(componentType_Transform)
{
	name = "Transform";
	numMax = 1;
	position = { 1, 1, 1 };
	rotation = { 0, 0, 0, 1 };
	scale = { 1, 1, 1 };
}

ComponentTransform::ComponentTransform(GameObject* argparent) : Component(componentType_Transform, argparent)
{
	name = "Transform";
	numMax = 1;
	position = { 1, 1, 1 };
	rotation = { 0, 0, 0, 1 };
	scale = { 1, 1, 1 };
}

ComponentTransform::ComponentTransform(componentType argtype, GameObject * argparent) : Component(componentType_Transform, argparent)
{
	name = "Transform";
	numMax = 1;
	position = { 1, 1, 1 };
	rotation = { 0, 0, 0, 1 };
	scale = { 1, 1, 1 };
}

ComponentTransform::ComponentTransform(GameObject * argparent, float3 position, Quat rotation, float3 scale) : Component(componentType_Transform, argparent)
{
	this->position = position; this->rotation = rotation; this->scale = scale;
}

void ComponentTransform::Update(float dt)
{
	if (!GetParent()->IsRoot())
	{
		ComponentTransform* parent_transform = (ComponentTransform*)GetParent()->GetParent()->GetTransform();

		WorldMatrix = WorldMatrix.FromTRS(position, rotation, scale);
		WorldMatrix = parent_transform->WorldMatrix * WorldMatrix;
	}
	else
	{
		WorldMatrix = float4x4::FromTRS(position, rotation, scale);
		for (std::vector<GameObject*>::iterator it = GetParent()->children.begin(); it != GetParent()->children.end(); ++it)
		{
			ComponentTransform* child_transform = (ComponentTransform*)(*it)->GetTransform();
			child_transform->UpdateMatrix();
		}
	}

	std::vector<Component*> components = GetParent()->FindComponents(componentType_Mesh);

	for (uint i = 0; i < components.size(); ++i)
	GetParent()->UpdateAABBFromMesh((ComponentMesh*)components[i]);

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
		if (pos[0] != position.x || pos[1] != position.y || pos[2] != position.z)
			Transformed = true;

		position.x = pos[0];
		position.y = pos[1];
		position.z = pos[2];
	}
	if (ImGui::DragFloat3("Rotation:", rot, 0.1f)) {
		if (rot[0] != RadToDeg(rotation.ToEulerXYZ().x) || rot[1] != RadToDeg(rotation.ToEulerXYZ().y) || rot[2] != RadToDeg(rotation.ToEulerXYZ().z))
			Transformed = true;

		rotation = rotation.FromEulerXYZ(DegToRad(rot[0]), DegToRad(rot[1]), DegToRad(rot[2]));
	}
	
	if (ImGui::DragFloat3("Scale:", sca, 0.1f)) {
		if (sca[0] != scale.x || sca[1] != scale.y || sca[2] != scale.z)
			Transformed = true;

		scale.x = sca[0];
		scale.y = sca[1];
		scale.z = sca[2];
	}

}

float4x4 ComponentTransform::GetWorldMatrix()
{
	return WorldMatrix;
}

float4x4 ComponentTransform::GetLocalMatrix() const
{
	return WorldMatrix.FromTRS(position, rotation, scale);
}

float* ComponentTransform::GetMatrix4x4() const
{
	const float4x4 retMat = WorldMatrix.FromTRS(position, rotation, scale);

	return retMat.Transposed().ptr();
}
