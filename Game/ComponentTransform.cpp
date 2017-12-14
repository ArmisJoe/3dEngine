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
	position_global = { 0, 0, 0 };
	rotation_global = { 0, 0, 0, 1 };
	scale_global = { 1, 1, 1 };
	rotinEuler_global = RadToDeg(rotation_global.ToEulerXYX());
	UpdateNeeded = true;
}

ComponentTransform::ComponentTransform(GameObject* argparent) : Component(componentType_Transform, argparent)
{
	typeName = "Transform";
	name = "Transform";
	numMax = 1;
	position_global = { 0, 0, 0 };
	rotation_global = { 0, 0, 0, 1 };
	scale_global = { 1, 1, 1 };
	rotinEuler_global = RadToDeg(rotation_global.ToEulerXYX());
	UpdateNeeded = true;
}

ComponentTransform::ComponentTransform(componentType argtype, GameObject * argparent) : Component(componentType_Transform, argparent)
{
	typeName = "Transform";
	name = "Transform";
	numMax = 1;
	position_global = { 0, 0, 0 };
	rotation_global = { 0, 0, 0, 1 };
	scale_global = { 1, 1, 1 };
	rotinEuler_global = RadToDeg(rotation_global.ToEulerXYX());
	UpdateNeeded = true;
}

ComponentTransform::ComponentTransform(GameObject * argparent, float3 position, Quat rotation, float3 scale) : Component(componentType_Transform, argparent)
{
	typeName = "Transform";
	name = "Transform";
	this->position_global = position; this->rotation_global = rotation; this->scale_global = scale;
	rotinEuler_global = RadToDeg(rotation_global.ToEulerXYX());
	UpdateNeeded = true;
}

float3 ComponentTransform::GetLocalPosition() const
{
	return localPos;
}

Quat ComponentTransform::GetLocalRotation() const
{
	return localRot;
}

float3 ComponentTransform::GetLocalScale() const
{
	return localScale;
}

void ComponentTransform::SetTransformMatrix()
{
	UpdateNeeded = true;
}

const float* ComponentTransform::GetGlobalTransformPtr()
{
	transform_matrix = transform_matrix.FromTRS(position_global, rotation_global, scale_global);
	return transform_matrix.Transposed().ptr();
}

int ComponentTransform::GetTransformID()const
{
	return transform_id;
}

float4x4 ComponentTransform::GetGlobalTransformMatrix()
{
	transform_matrix = transform_matrix.FromTRS(position_global, rotation_global, scale_global);
	return transform_matrix;
}

float4x4 ComponentTransform::GetLocalTransformMatrix()
{
	UpdateNeeded = true;
	float4x4 lclmat = lclmat.FromTRS(localPos, localRot, localScale);
	return lclmat;
}

void ComponentTransform::SetIdentityTransform()
{
	if (GetParent() != nullptr && !GetParent()->IsStatic())
	{
		SetPosition({ 0,0,0 });
		Quat rot;
		SetRotation({ 0.0f, 0.0f, 0.0f });
		SetScale({ 1,1,1 });
	}
	UpdateNeeded = true;
}

void ComponentTransform::Update(float dt)
{
	if (UpdateNeeded == true && GetParent() != nullptr && GetParent()->GetParent() != nullptr)
	{
		SetLocalTrans();
	}
}

void ComponentTransform::SetLocalTrans()
{
	if (GetParent() != nullptr && GetParent()->GetParent() != nullptr)
	{
		ComponentTransform* parentTrans = (ComponentTransform*)GetParent()->GetParent()->GetTransform();
		if (parentTrans != nullptr)
		{
			localPos = position_global - parentTrans->position_global;
			localRot = rotation_global * parentTrans->rotation_global.Conjugated();
			localScale = scale_global.Mul(parentTrans->scale_global.Recip());
			GetParent()->GetParent()->GetTransform()->SetLocalTrans();
			UpdateNeeded = false;
		}
	}
}

void ComponentTransform::UpdateChildren(float3 pos_offset, float3 scale_offset, Quat rot_offset)
{
	if (GetParent() != nullptr)
	{
		if (!GetParent()->children.empty())
		{
			for (std::vector<GameObject*>::iterator it = GetParent()->children.begin(); it != GetParent()->children.end(); ++it)
			{
				GameObject* curr = (*it);
				ComponentTransform* transform = curr->GetTransform();

				transform->SetPositionFromParent(pos_offset);
				transform->SetScaleFromParent(scale_offset);
				transform->SetRotationFromParent(rot_offset);

				transform->UpdateChildren(pos_offset, scale_offset, rot_offset);
			}
		}
	}
	//Quat old_global, new_global;
	//Quat ret = old_global - new_global;

}

void ComponentTransform::SetGlobalPosition(float3 newpos)
{
	if (GetParent() == nullptr)
		return;

	float3 dpos = newpos - position_global;

	position_global = newpos;

	for (int i = 0; i < GetParent()->children.size(); i++) {
		ComponentTransform* cTrans = GetParent()->children[i]->GetTransform();
		cTrans->SetGlobalPosition(cTrans->GetPosition() + dpos);
	}

	transform_modified = true;
}

void ComponentTransform::SetGlobalRotation(Quat newrot, bool toSelf)
{
	if (GetParent() == nullptr)
		return;

	float3 eulerrot = rotation_global.ToEulerXYZ();
	float3 eulernewrot = newrot.ToEulerXYZ();

	float3 drot = eulernewrot - eulerrot;
	if(toSelf)
		rotation_global = newrot;

	for (int i = 0; i < GetParent()->children.size(); i++) {
		ComponentTransform* cTrans = GetParent()->children[i]->GetTransform();
		// Children Pos Set
		float3 cnewpos;
		float3 dirdistance = cTrans->GetPosition() - GetParent()->GetTransform()->GetPosition();
		float distance = dirdistance.LengthSq();
		// drot == dangle !

		float3 newdistance = dirdistance;
		// rot X axis
		newdistance.x = newdistance.x * Cos(drot.x) - newdistance.y * Sin(drot.x);
		newdistance.y = newdistance.x * Sin(drot.x) + newdistance.y * Cos(drot.x);
		newdistance.z = newdistance.z;
		// rot Y axis
		newdistance.x = newdistance.x * Cos(drot.y) + newdistance.z * Sin(drot.y);
		newdistance.y = newdistance.y;
		newdistance.z = -newdistance.x * Sin(drot.y) + newdistance.z * Cos(drot.y);
		// rot Z axis
		newdistance.x = newdistance.x;
		newdistance.y = newdistance.y * Cos(drot.z) - newdistance.z * Sin(drot.z);
		newdistance.z = newdistance.y * Sin(drot.z) + newdistance.z * Cos(drot.z);

		cnewpos = GetParent()->GetTransform()->GetPosition() + newdistance;

		cTrans->SetGlobalPosition(cnewpos);

		// Children Recursivity
		float3 ceulerrot = cTrans->GetRotation().ToEulerXYZ();
		float3 totalrot = ceulerrot + drot;
		Quat totalquatrot = Quat::FromEulerXYZ(totalrot.x, totalrot.y, totalrot.x);
		cTrans->SetGlobalRotation(totalquatrot, false);
	}

	transform_modified = true;

}

void ComponentTransform::SetGlobalScale(float3 newsca)
{
	if (GetParent() == nullptr)
		return;

	float3 dsca = newsca - scale_global;

	scale_global = newsca;

	for (int i = 0; i < GetParent()->children.size(); i++) {
		ComponentTransform* cTrans = GetParent()->children[i]->GetTransform();
		cTrans->SetGlobalScale(cTrans->GetScale() + dsca);
	}

	transform_modified = true;
}

void ComponentTransform::SetRotationHyerarchy(Quat newrot)
{
	float3 nRot = float3::zero;

	float3 rot = RadToDeg(newrot.ToEulerXYZ());

	nRot[0] = DegToRad(rot[0]) - DegToRad(rotinEuler_global.x);
	nRot[1] = DegToRad(rot[1]) - DegToRad(rotinEuler_global.y);
	nRot[2] = DegToRad(rot[2]) - DegToRad(rotinEuler_global.z);

	Quat off_rot = Quat::FromEulerXYZ((nRot[0]),/* DegToRad*/(nRot[1]), (nRot[2]));

	rotation_global = rotation_global * off_rot;

	rotinEuler_global.x = rot[0];
	rotinEuler_global.y = rot[1];
	rotinEuler_global.z = rot[2];


	transform_modified = true;
}


void ComponentTransform::SetPosition(const float3 & _position)
{
	if (GetParent() != nullptr && !GetParent()->IsStatic())
	{
		position_global = _position;
		transform_modified = true;
		UpdateNeeded = true;
	}

}

void ComponentTransform::SetRotation(const float3& _rotation)
{
	if (GetParent() != nullptr && !GetParent()->IsStatic())
	{
		Quat mod = Quat::FromEulerXYZ(_rotation.x, _rotation.y, _rotation.z);
		rotation_global = mod;
		rotinEuler_global = RadToDeg(rotation_global.ToEulerXYX());
		transform_modified = true;
		UpdateNeeded = true;
	}
}

void ComponentTransform::SetRotation(const Quat& _rotation)
{
	if (GetParent() != nullptr && !GetParent()->IsStatic())
	{
		rotation_global = _rotation;
		transform_modified = true;
		UpdateNeeded = true;
		rotinEuler_global = RadToDeg(rotation_global.ToEulerXYX());
	}
}

void ComponentTransform::LoadPosition(const float3& argposition)
{
	position_global = argposition;
	transform_modified = true;
	UpdateNeeded = true;
}
void ComponentTransform::LoadRotation(const Quat& argrotation)
{
	rotation_global = argrotation;
	rotinEuler_global = RadToDeg(rotation_global.ToEulerXYX());
	transform_modified = true;
	UpdateNeeded = true;
}
void ComponentTransform::LoadScale(const float3& argscale)
{
	scale_global = argscale;
	transform_modified = true;
	UpdateNeeded = true;
}

void ComponentTransform::SetScale(const float3 & _scale)
{
	if (GetParent() != nullptr && !GetParent()->IsStatic())
	{
		scale_global = _scale;
		transform_modified = true;
		UpdateNeeded = true;
	}
}

float3 ComponentTransform::GetPosition()
{
	return position_global;
}

Quat ComponentTransform::GetRotation()
{
	return rotation_global;
}

float3 ComponentTransform::GetScale()
{
	return scale_global;
}

void ComponentTransform::SetPositionFromParent(const float3 & position)
{
	position_global += position;
	transform_modified = true;
}
void ComponentTransform::SetScaleFromParent(const float3 & scale)
{
	scale_global += scale;
	transform_modified = true;
}
void ComponentTransform::ChangeLocalPosition(const float3 & position)
{
	float3 pos = float3::zero;
	pos = position - localPos;
	localPos = position;
	position_global += pos;

	transform_modified = true;
}

void ComponentTransform::ChangeLocalRotation(const Quat & rotation)
{

	float3 rot = float3::zero;

	rot = RadToDeg(rotation.ToEulerXYZ()) - RadToDeg(localRot.ToEulerXYZ());

	localRot = rotation;

	rotinEuler_global += rot;

	rotation_global = Quat::FromEulerXYZ(DegToRad(rotinEuler_global.x), DegToRad(rotinEuler_global.y), DegToRad(rotinEuler_global.z));

	transform_modified = true;
}

void ComponentTransform::ChangeLocalScale(const float3 & scale)
{
	float3 sca = float3::zero;
	sca = scale - localScale;
	localScale = scale;
	scale_global += sca;
	transform_modified = true;
}

void ComponentTransform::SetRotationFromParent(const Quat& rot)
{
	rotation_global = rotation_global * rot.Normalized();
	transform_modified = true;
	rotinEuler_global = RadToDeg(rotation_global.ToEulerXYX());
}


void ComponentTransform::OnEditor()
{
	//UpdateNeeded = true;
	ImGui::TextColored(COLOR_YELLOW, "Global Transform:");
	float pos[3];
	float rot[3];
	float sca[3];

	pos[0] = position_global.x;
	pos[1] = position_global.y;
	pos[2] = position_global.z;

	rot[0] = (rotinEuler_global.x);
	rot[1] = (rotinEuler_global.y);
	rot[2] = (rotinEuler_global.z);

	sca[0] = scale_global.x;
	sca[1] = scale_global.y;
	sca[2] = scale_global.z;

	float3 off_pos = float3::zero, off_sca = float3::zero;
	Quat off_rot = Quat::identity;

	if (ImGui::DragFloat3("Position:", pos, 0.1f)) {
		if (GetParent() != nullptr && !GetParent()->IsStatic())
		{
			off_pos.x = pos[0] - position_global.x;
			off_pos.y = pos[1] - position_global.y;
			off_pos.z = pos[2] - position_global.z;

			position_global += off_pos;

			transform_modified = true;
		}
	}
	if (ImGui::DragFloat3("Rotation:", rot, 0.1f)) {
		if (GetParent() != nullptr && !GetParent()->IsStatic())
		{
			float3 nRot = float3::zero;

			nRot[0] = DegToRad(rot[0]) - DegToRad(rotinEuler_global.x);
			nRot[1] = DegToRad(rot[1]) - DegToRad(rotinEuler_global.y);
			nRot[2] = DegToRad(rot[2]) - DegToRad(rotinEuler_global.z);

			off_rot = Quat::FromEulerXYZ((nRot[0]),/* DegToRad*/(nRot[1]), (nRot[2]));

			rotation_global = rotation_global * off_rot;

			rotinEuler_global.x = rot[0];
			rotinEuler_global.y = rot[1];
			rotinEuler_global.z = rot[2];


			transform_modified = true;
		}
	}
	if (ImGui::DragFloat3("Scale:", sca, 0.1f)) {
		if (GetParent() != nullptr && !GetParent()->IsStatic())
		{
			off_sca.x = sca[0] - scale_global.x;
			off_sca.y = sca[1] - scale_global.y;
			off_sca.z = sca[2] - scale_global.z;

			scale_global += off_sca;

			transform_modified = true;
		}
	}

	if (off_pos.x != 0 || off_pos.y != 0 || off_pos.z != 0 || off_sca.x != 0 || off_sca.y != 0 || off_sca.z != 0 || off_rot.x != 0 || off_rot.y != 0 || off_rot.z != 0)
		UpdateChildren(off_pos, off_sca, off_rot);

	ImGui::TextColored(COLOR_YELLOW, "Local Transform:");
	float lpos[3];
	float lrot[3];
	float lsca[3];

	lpos[0] = localPos.x;
	lpos[1] = localPos.y;
	lpos[2] = localPos.z;

	lrot[0] = RadToDeg(localRot.ToEulerXYZ().x);
	lrot[1] = RadToDeg(localRot.ToEulerXYZ().y);
	lrot[2] = RadToDeg(localRot.ToEulerXYZ().z);

	lsca[0] = localScale.x;
	lsca[1] = localScale.y;
	lsca[2] = localScale.z;

	if (ImGui::DragFloat3("LPosition:", lpos, 0.1f)) {
		if (GetParent() != nullptr && !GetParent()->IsStatic())
		{
			/*localPos.x = lpos[0];
			localPos.y = lpos[1];
			localPos.z = lpos[2];
			transform_modified = true;
			UpdateNeeded = true;*/
		}
	}
	if (ImGui::DragFloat3("LRotation:", lrot, 0.1f)) {
		if (GetParent() != nullptr && !GetParent()->IsStatic())
		{
			/*	rotation_global = rotation_global.FromEulerXYZ(DegToRad(lrot[0]), DegToRad(lrot[1]), DegToRad(lrot[2]));
			rotinEuler_global = RadToDeg(rotation_global.ToEulerXYX());
			transform_modified = true;
			UpdateNeeded = true;*/
		}
	}

	if (ImGui::DragFloat3("LScale:", lsca, 0.1f)) {
		if (GetParent() != nullptr && !GetParent()->IsStatic())
		{
			/*scale_global.x = lsca[0];
			scale_global.y = lsca[1];
			scale_global.z = lsca[2];
			transform_modified = true;
			UpdateNeeded = true;*/
		}
	}



	// ImGuizmo
	if (App->input->GetKey(SDL_SCANCODE_LCTRL) == KEY_REPEAT & GetParent() != nullptr && !GetParent()->IsStatic())
	{
		App->camera->SetCameraActive(false);
		ImGuizmo::Enable(true);

		if (ImGui::Button("Translate") || App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
			mCurrentGuizmoOperation = ImGuizmo::TRANSLATE;
		ImGui::SameLine();
		if (ImGui::Button("Rotate") || App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
			mCurrentGuizmoOperation = ImGuizmo::ROTATE;
		ImGui::SameLine();
		if (ImGui::Button("Scale") || App->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN)
			mCurrentGuizmoOperation = ImGuizmo::SCALE;


		ImGuiIO& io = ImGui::GetIO();
		ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);


		float4x4 view_matrix = App->camera->main_camera->GetFrustum().ViewProjMatrix();
		float4x4 view_matrix1 = App->camera->main_camera->GetFrustum().ViewMatrix();
		view_matrix1.Transpose();
		view_matrix.Transpose();

		float4x4 trs_matrix = GetGlobalTransformMatrix().Transposed();

		ImGuizmo::Manipulate(view_matrix1.ptr(), view_matrix.ptr(), mCurrentGuizmoOperation, mCurrentGuizmoMode, trs_matrix.ptr());

		if (ImGuizmo::IsUsing())
		{
			ImGuizmo::DecomposeMatrixToComponents(trs_matrix.ptr(), position_global.ptr(), (rotinEuler_global).ptr(), scale_global.ptr());
			trs_matrix.Transpose();
			ImGuizmo::RecomposeMatrixFromComponents((float*)position_global.ptr(), (float*)(rotinEuler_global).ptr(), (float*)scale_global.ptr(), trs_matrix.ptr());
			trs_matrix.Transpose();

			rotation_global = Quat::FromEulerXYZ(DegToRad(rotinEuler_global.x), DegToRad(rotinEuler_global.y), DegToRad(rotinEuler_global.z));

			rotinEuler_global = (rotinEuler_global);
			transform_modified = true;
		}
	}
	else {
		ImGuizmo::Enable(false);
		App->camera->SetCameraActive(true);
	}
}


void ComponentTransform::Serialize(JSON_Doc* doc) {
	if (doc == nullptr)
		return;

	// Type
	doc->SetNumber("type", type);
	doc->SetNumber("parentUID", (parent != nullptr) ? parent->GetUID() : -1);
	// Pos
	doc->SetNumber("position.x", position_global.x);
	doc->SetNumber("position.y", position_global.y);
	doc->SetNumber("position.z", position_global.z);
	// Rot
	doc->SetNumber("rotation.x", rotation_global.x);
	doc->SetNumber("rotation.y", rotation_global.y);
	doc->SetNumber("rotation.z", rotation_global.z);
	doc->SetNumber("rotation.w", rotation_global.w);
	// Scale
	doc->SetNumber("scale.x", scale_global.x);
	doc->SetNumber("scale.y", scale_global.y);
	doc->SetNumber("scale.z", scale_global.z);

	transform_modified = true;
}