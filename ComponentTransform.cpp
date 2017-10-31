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
		rotation.x *= rot[0];
		rotation.y *= rot[1];
		rotation.z *= rot[2];
	}
	
	if (ImGui::DragFloat3("Scale:", sca, 0.1f)) {
		scale.x = sca[0];
		scale.y = sca[1];
		scale.z = sca[2];
	}

}

float* ComponentTransform::GetMatrix4x4() const
{

	Matrix4x4.FromTRS(GetGlobalPos(), GetGlobalRot(), GetGlobalScale());

	return Matrix4x4.Transposed().ptr();
}

float3 ComponentTransform::GetGlobalPos() const
{
	float3 GP = position;

	//if (parent != nullptr) {
	//	if (parent->GetParent() != nullptr) {
	//		if (!parent->GetParent()->FindComponents(componentType_Transform).empty()) {
	//			ComponentTransform* ParentTrans = (ComponentTransform*)parent->GetParent()->FindComponents(componentType_Transform)[0];
	//			GP += ParentTrans->GetGlobalPos();
	//		}
	//	}
	//}

	return GP;
}

Quat ComponentTransform::GetGlobalRot() const
{
	Quat GR = rotation;

	//if (parent != nullptr) {
	//	GameObject* GoParent = parent->GetParent();
	//	if (!GoParent->FindComponents(componentType_Transform).empty()) {
	//		ComponentTransform* ParentTrans = (ComponentTransform*)GoParent->FindComponents(componentType_Transform)[0];
	//		GR.Mul(ParentTrans->GetGlobalRot());
	//	}
	//
	//}

	return GR;
}

float3 ComponentTransform::GetGlobalScale() const
{
	float3 GS = scale;

	//if (parent != nullptr) {
	//	GameObject* GoParent = parent->GetParent();
	//	if (!GoParent->FindComponents(componentType_Transform).empty()) {
	//		ComponentTransform* ParentTrans = (ComponentTransform*)GoParent->FindComponents(componentType_Transform)[0];
	//		GS += ParentTrans->GetGlobalScale();
	//	}
	//}

	return GS;
}
