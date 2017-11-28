#include "ImGui\imgui.h"
#include "ImGuizmo\ImGuizmo.h"
#include "ComponentTransform.h"
#include "MathGeoLib\MathGeoLib.h"

#include "Application.h"
#include "GameObject.h"


ComponentTransform::ComponentTransform() : Component(componentType_Transform)
{
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
	this->position_global = position; this->rotation_global = rotation; this->scale_global = scale;
	rotinEuler_global = RadToDeg(rotation_global.ToEulerXYX());
	UpdateNeeded = true;
}

float3 ComponentTransform::GetLocalPosition()
{
	SetLocalTrans();
	return position_global;
}

Quat ComponentTransform::GetLocalRotation()
{
	SetLocalTrans();
	return rotation_global;
}

float3 ComponentTransform::GetLocalScale()
{
	SetLocalTrans();
	return scale_global;
}

void ComponentTransform::SetTransformMatrix()
{

}

const float* ComponentTransform::GetGlobalTransformPtr()
{
	transform_matrix = transform_matrix.FromTRS(position_global, rotation_global, scale_global);
	SetLocalTrans();
	UpdateNeeded = true;
	return transform_matrix.Transposed().ptr();
}

int ComponentTransform::GetTransformID()const
{
	return transform_id;
}

float4x4 ComponentTransform::GetGlobalTransformMatrix()
{
	transform_matrix = transform_matrix.FromTRS(position_global, rotation_global, scale_global);
	SetLocalTrans();
	UpdateNeeded = true;
	return transform_matrix;
}

float4x4 ComponentTransform::GetLocalTransformMatrix()
{
	SetLocalTrans();
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
	if (UpdateNeeded == true)
	{
		SetLocalTrans();
		UpdateNeeded = false;
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
			}
		}	
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


void ComponentTransform::OnEditor()
{
	UpdateNeeded = true;
	ImGui::TextColored(COLOR_YELLOW, "Global Transform:");
	float pos[3];
	float rot[3];
	float sca[3];

	pos[0] = position_global.x;
	pos[1] = position_global.y;
	pos[2] = position_global.z;

	rot[0] = RadToDeg(rotation_global.ToEulerXYZ().x);
	rot[1] = RadToDeg(rotation_global.ToEulerXYZ().y);
	rot[2] = RadToDeg(rotation_global.ToEulerXYZ().z);

	sca[0] = scale_global.x;
	sca[1] = scale_global.y;
	sca[2] = scale_global.z;

	if (ImGui::DragFloat3("Position:", pos, 0.1f)) {
		if (GetParent() != nullptr && !GetParent()->IsStatic())
		{
			position_global.x = pos[0];
			position_global.y = pos[1];
			position_global.z = pos[2];
			transform_modified = true;
			UpdateNeeded = true;
		}
	}
	if (ImGui::DragFloat3("Rotation:", rot, 0.1f)) {
		if (GetParent() != nullptr && !GetParent()->IsStatic())
		{
			rotation_global = rotation_global.FromEulerXYZ(DegToRad(rot[0]), DegToRad(rot[1]), DegToRad(rot[2]));
			rotinEuler_global = RadToDeg(rotation_global.ToEulerXYX());
			transform_modified = true;
			UpdateNeeded = true;
		}
	}

	if (ImGui::DragFloat3("Scale:", sca, 0.1f)) {
		if (GetParent() != nullptr && !GetParent()->IsStatic())
		{
			scale_global.x = sca[0];
			scale_global.y = sca[1];
			scale_global.z = sca[2];
			transform_modified = true;
			UpdateNeeded = true;
		}
	}

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
			ImGuizmo::DecomposeMatrixToComponents(trs_matrix.ptr(), position_global.ptr(), (float*) RadToDeg(rotation_global.ToEulerXYZ()).ptr(), scale_global.ptr());
			trs_matrix.Transpose();
			ImGuizmo::RecomposeMatrixFromComponents((float*)position_global.ptr(), RadToDeg(rotation_global.ToEulerXYZ()).ptr(), (float*)scale_global.ptr(), trs_matrix.ptr());
			trs_matrix.Transpose();
			transform_modified = true;
			UpdateNeeded = true;
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