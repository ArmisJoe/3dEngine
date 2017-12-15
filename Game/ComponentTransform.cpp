#include "ImGui\imgui.h"
#include "ImGuizmo\ImGuizmo.h"
#include "ComponentTransform.h"
#include "MathGeoLib\MathGeoLib.h"

#include "Application.h"
#include "GameObject.h"


ComponentTransform::ComponentTransform() : Component(componentType_Transform)
{
	typeName = "Transform";
	name = "Transform";
	numMax = 1;
	transform = float4x4::FromTRS(position, rotation, scale);
	UpdateEulerAngles();
}

ComponentTransform::ComponentTransform(GameObject* argparent) : Component(componentType_Transform, argparent)
{
	typeName = "Transform";
	name = "Transform";
	numMax = 1;
	transform = float4x4::FromTRS(position, rotation, scale);
	UpdateEulerAngles();

}

ComponentTransform::ComponentTransform(componentType argtype, GameObject * argparent) : Component(componentType_Transform, argparent)
{
	typeName = "Transform";
	name = "Transform";
	numMax = 1;
	transform = float4x4::FromTRS(position, rotation, scale);
	UpdateEulerAngles();

}

ComponentTransform::ComponentTransform(GameObject * argparent, float3 position, Quat rotation, float3 scale) : Component(componentType_Transform, argparent)
{
	typeName = "Transform";
	name = "Transform";
	transform = float4x4::FromTRS(position, rotation, scale);
	UpdateEulerAngles();

	/*this->position_global = position; this->rotation_global = rotation; this->scale_global = scale;
	rotinEuler_global = RadToDeg(rotation_global.ToEulerXYX());
	UpdateNeeded = true;*/
}

void ComponentTransform::Update(float dt)
{
	/*if (UpdateNeeded == true && GetParent() != nullptr && GetParent()->GetParent() != nullptr)
	{
		SetLocalTrans();
	}*/
}


void ComponentTransform::Serialize(JSON_Doc* doc) {
	if (doc == nullptr)
		return;

	// Type
	doc->SetNumber("type", type);
	doc->SetNumber("parentUID", (parent != nullptr) ? parent->GetUID() : -1);
	// Pos
	doc->SetNumber("position.x", position.x);
	doc->SetNumber("position.y", position.y);
	doc->SetNumber("position.z", position.z);
	// Rot
	doc->SetNumber("rotation.x", rotation.x);
	doc->SetNumber("rotation.y", rotation.y);
	doc->SetNumber("rotation.z", rotation.z);
	doc->SetNumber("rotation.w", rotation.w);
	// Scale
	doc->SetNumber("scale.x", scale.x);
	doc->SetNumber("scale.y", scale.y);
	doc->SetNumber("scale.z", scale.z);
}

float4x4 ComponentTransform::GetTransform() const
{
	return transform;
}

float3 ComponentTransform::GetPosition() const
{
	return position;
}

Quat ComponentTransform::GetQuatRotation() const
{
	return rotation;
}

float3 ComponentTransform::GetEulerRotation() const
{
	return rotinEuler;
}

float3 ComponentTransform::GetScale() const
{
	return scale;
}

float4x4 ComponentTransform::GetGlobalTransform() const
{
	return global_transform;
}

float4x4 ComponentTransform::GetGlobalTransformT() const
{
	return global_transform.Transposed();
}

float3 ComponentTransform::GetGlobalPosition() const
{
	float3 pos, sca;
	Quat rot;
	GetGlobalTransform().Decompose(pos, rot, sca);
	return pos;
}

Quat ComponentTransform::GetGlobalQuatRotation() const
{
	float3 pos, sca;
	Quat rot;
	GetGlobalTransform().Decompose(pos, rot, sca);
	return rot;
}

float3 ComponentTransform::GetGlobalEulerRotation() const
{
	float3 pos, sca;
	Quat rot;
	GetGlobalTransform().Decompose(pos, rot, sca);
	return rot.ToEulerXYZ();
}

float3 ComponentTransform::GetGlobalScale() const
{
	float3 pos, sca;
	Quat rot;
	GetGlobalTransform().Decompose(pos, rot, sca);
	return sca;
}

void ComponentTransform::UpdateEulerAngles()
{
	rotinEuler = rotation.ToEulerXYZ();
	rotinEuler *= RADTODEG;
}

void ComponentTransform::UpdateTransform()
{
	transform = float4x4::FromTRS(position, rotation, scale);
	transform_modified = true;
}


void ComponentTransform::SetPosition(float3 position)
{
	float3 diff = position - this->position;
	this->position = position;
	//for (int i = 0; i < GetParent()->children.size(); i++) {
	//	ComponentTransform* cTrans = GetParent()->children[i]->GetTransform();
	//	cTrans->SetPosition(cTrans->GetPosition() + diff);
	//}
	UpdateTransform();
}

void ComponentTransform::SetScale(float3 scale)
{
	this->scale = scale;
	UpdateTransform();
}

void ComponentTransform::SetQuatRotation(Quat rotation)
{
	Quat drot = rotation * this->rotation.Inverted();

	this->rotation = rotation;

	for (int i = 0; i < GetParent()->children.size(); i++) {
		ComponentTransform* cTrans = GetParent()->children[i]->GetTransform();
		// Children Pos Set
		float3 cnewpos;
		float3 dirdistance = cTrans->GetPosition() - GetPosition();
		float distance = dirdistance.Length();
		// drot == dangle !
		float3 newdistance = drot * dirdistance;
		cnewpos = newdistance + GetPosition();

		if (!newdistance.Equals(dirdistance))
			cTrans->SetPosition(cnewpos);

		// Children Recursivity
		//cTrans->SetQuatRotation(this->rotation);

	}

	UpdateEulerAngles();
	UpdateTransform();
}

void ComponentTransform::SetEulerRotation(float3 euler_angles)
{
	float3 delta = (euler_angles - rotinEuler) * DEGTORAD;
	Quat quaternion_rotation = Quat::FromEulerXYZ(delta.x, delta.y, delta.z);
	rotation = rotation * quaternion_rotation;
	rotinEuler = euler_angles;
	UpdateTransform();
}

void ComponentTransform::SetGlobalTransform(float4x4 transform)
{
	if (GetParent() != nullptr)
	{
		if (GetParent()->GetParent() != nullptr)
		{
			float4x4 localTransform = GetParent()->GetParent()->GetTransform()->GetGlobalTransform().Inverted() * transform;
			this->transform = localTransform;
			global_transform = transform;
			transform_modified = true;
		}
	}
}

void ComponentTransform::OnUpdateTransform(const float4x4 & global, const float4x4 & parent_global)
{
	global_transform = parent_global.Inverted() * transform;
	transform_modified = true;
}

void ComponentTransform::LoadGlobalTransform(float4x4 transform)
{
	global_transform = transform;
	transform_modified = true;
}

void ComponentTransform::OnEditor()
{
	//UpdateNeeded = true;
	ImGui::TextColored(COLOR_YELLOW, "Global Transform:");
	float pos[3];
	float rot[3];
	float sca[3];

	float3 posi, skya, rotis;
	Quat q;

	global_transform.Decompose(posi, q, skya);

	rotis = RadToDeg(q.ToEulerXYZ());

	pos[0] = posi.x;
	pos[1] = posi.y;
	pos[2] = posi.z;

	rot[0] = (rotis.x);
	rot[1] = (rotis.y);
	rot[2] = (rotis.z);

	sca[0] = skya.x;
	sca[1] = skya.y;
	sca[2] = skya.z;

	float3 off_pos = float3::zero, off_sca = float3::zero;
	Quat off_rot = Quat::identity;

	if (ImGui::DragFloat3("Position:", pos, 0.1f)) {
		if (GetParent() != nullptr && !GetParent()->IsStatic())
		{
		}
	}
	if (ImGui::DragFloat3("Rotation:", rot, 0.1f)) {
		if (GetParent() != nullptr && !GetParent()->IsStatic())
		{
		}
	}
	if (ImGui::DragFloat3("Scale:", sca, 0.1f)) {
		if (GetParent() != nullptr && !GetParent()->IsStatic())
		{
		}
	}

	ImGui::TextColored(COLOR_YELLOW, "Local Transform:");
	float lpos[3];
	float lrot[3];
	float lsca[3];

	lpos[0] = position.x;
	lpos[1] = position.y;
	lpos[2] = position.z;

	lrot[0] = rotinEuler.x;
	lrot[1] = rotinEuler.y;
	lrot[2] = rotinEuler.z;

	lsca[0] = scale.x;
	lsca[1] = scale.y;
	lsca[2] = scale.z;

	if (ImGui::DragFloat3("LPosition:", lpos, 0.1f)) {
		if (GetParent() != nullptr && !GetParent()->IsStatic())
		{
		}
	}
	if (ImGui::DragFloat3("LRotation:", lrot, 0.1f)) {
		if (GetParent() != nullptr && !GetParent()->IsStatic())
		{
		}
	}

	if (ImGui::DragFloat3("LScale:", lsca, 0.1f)) {
		if (GetParent() != nullptr && !GetParent()->IsStatic())
		{
		}
	}
}