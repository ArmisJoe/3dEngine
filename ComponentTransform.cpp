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

ComponentTransform::ComponentTransform(componentType argtype, GameObject * argparent) : Component(argtype, argparent)
{
	name = "Transform";
	numMax = 1;
	position = { 0, 0, 0 };
	rotation = { 0, 0, 0, 0 };
	scale = { 1, 1, 1 };
}

void ComponentTransform::OnEditor()
{
	float pos[3];
	float rot[3];
	float sca[3];

	pos[0] = position.x;
	pos[1] = position.y;
	pos[2] = position.z;

	float3 r = RadToDeg(rotation.ToEulerXYZ());
	rot[0] = (r.x);
	rot[1] = (r.y);
	rot[2] = (r.z);

	sca[0] = scale.x;
	sca[1] = scale.y;
	sca[2] = scale.z;

	if (ImGui::DragFloat3("Position:", pos, 0.1f)) { 
		position.x = pos[0];
		position.y = pos[1];
		position.z = pos[2];
	}
	if (ImGui::DragFloat3("Rotation:", rot, 0.1f)) {
		float3 tmp = DegToRad(float3(rot[0], rot[1], rot[2]));
		rotation = rotation.FromEulerXYZ(tmp.x, tmp.y, tmp.z);
	}
	
	if (ImGui::DragFloat3("Scale:", sca, 0.1f)) {
		scale.x = sca[0];
		scale.y = sca[1];
		scale.z = sca[2];
	}

}

float* ComponentTransform::GetMatrix4x4() const
{

	const float4x4 retMat = Matrix4x4.FromTRS(position, rotation, scale);

	return retMat.Transposed().ptr();
}
