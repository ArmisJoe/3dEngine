#include "ImGui\imgui.h"
#include "ImGuizmo\ImGuizmo.h"
#include "ComponentTransform.h"
#include "Application.h"
#include "GameObject.h"


ComponentTransform::ComponentTransform() : Component(componentType_Transform)
{
	name = "Transform";
	numMax = 1;
	position = { 0, 0, 0 };
	rotation = { 0, 0, 0, 1 };
	scale = { 1, 1, 1 };
}

ComponentTransform::ComponentTransform(GameObject* argparent) : Component(componentType_Transform, argparent)
{
	name = "Transform";
	numMax = 1;
	position = { 0, 0, 0 };
	rotation = { 0, 0, 0, 1 };
	scale = { 1, 1, 1 };
}

ComponentTransform::ComponentTransform(componentType argtype, GameObject * argparent) : Component(componentType_Transform, argparent)
{
	name = "Transform";
	numMax = 1;
	position = { 0, 0, 0 };
	rotation = { 0, 0, 0, 1 };
	scale = { 1, 1, 1 };
}

ComponentTransform::ComponentTransform(GameObject * argparent, float3 position, Quat rotation, float3 scale) : Component(componentType_Transform, argparent)
{
	this->position = position; this->rotation = rotation; this->scale = scale;
}

float3 ComponentTransform::GetLocalPosition()const
{
	return position;
}

Quat ComponentTransform::GetLocalRotation()const
{
	return rotation;
}

float3 ComponentTransform::GetLocalScale()const
{
	return scale;
}

void ComponentTransform::SetTransformMatrix()
{

}

const float* ComponentTransform::GetLocalTransform()
{
	transform_matrix = transform_matrix.FromTRS(position, rotation, scale);
	return transform_matrix.Transposed().ptr();
}

int ComponentTransform::GetTransformID()const
{
	return transform_id;
}

float4x4 ComponentTransform::GetTransformMatrix()
{
	transform_matrix = transform_matrix.FromTRS(position, rotation, scale);
	return transform_matrix;
}

void ComponentTransform::SetIdentityTransform()
{
	SetPosition({ 0,0,0 });
	Quat rot;
	SetRotation({ 0.0f, 0.0f, 0.0f });
	SetScale({ 1,1,1 });
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
			child_transform->Update(dt);
		}
	}
}

void ComponentTransform::SetPosition(const float3 & _position)
{
	if (GetParent()->IsStatic() == false)
	{
		position = _position;
		transform_modified = true;
	}

}

void ComponentTransform::SetRotation(const float3& _rotation)
{
	if (GetParent()->IsStatic() == false)
	{
		Quat mod = Quat::FromEulerXYZ(_rotation.x, _rotation.y, _rotation.z);
		rotation = mod;

		transform_modified = true;

	}
}


void ComponentTransform::SetScale(const float3 & _scale)
{
	if (GetParent()->IsStatic() == false)
	{
		scale = _scale;

		transform_modified = true;
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

	// ImGuizmo
/*

	ImGuizmo::Enable(true);

	static ImGuizmo::OPERATION mCurrentGizmoOperation(ImGuizmo::ROTATE);
	static ImGuizmo::MODE mCurrentGizmoMode(ImGuizmo::WORLD);

	static bool local_opt = false;

	ImGui::Separator();
	if (ImGui::Checkbox("Local", &local_opt))
	{
		if (local_opt)
			mCurrentGizmoMode = ImGuizmo::LOCAL;
		else mCurrentGizmoMode = ImGuizmo::WORLD;
	}

	/*if (ImGui::Button("Translate") || App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
		mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
	ImGui::SameLine();
	if (ImGui::Button("Rotate") || App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
		mCurrentGizmoOperation = ImGuizmo::ROTATE;
	ImGui::SameLine();
	if (ImGui::Button("Scale") || App->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN)
		mCurrentGizmoOperation = ImGuizmo::SCALE;


	ImGuiIO& io = ImGui::GetIO();
	ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);


	float4x4 view_matrix = App->camera->frustum.ViewMatrix();
	float4x4 proj_matrix = App->camera->frustum.ProjectionMatrix();
	view_matrix.Transpose();
	proj_matrix.Transpose();


	float4x4 trs_matrix = transform_matrix;
	ImGuizmo::Manipulate(view_matrix.ptr(), proj_matrix.ptr(), mCurrentGizmoOperation, mCurrentGizmoMode, trs_matrix.ptr());

	if (ImGuizmo::IsUsing())
	{
		trs_matrix.Transpose();
		trs_matrix = GetParentTransform().Transposed().Inverted() * trs_matrix;

		float3 new_pos;
		float3 new_scale;
		Quat new_q;
		trs_matrix.Decompose(new_pos, new_q, new_scale);

		if (mCurrentGizmoOperation == ImGuizmo::TRANSLATE)
			SetTranslation(new_pos.x, new_pos.y, new_pos.z);
		if (mCurrentGizmoOperation == ImGuizmo::SCALE)
			SetScale(new_scale.x, new_scale.y, new_scale.z);
		if (mCurrentGizmoOperation == ImGuizmo::ROTATE)
		{
			rot_in_euler = new_q.ToEulerXYZ()*RADTODEG;
			SetRotation(new_q);
		}

	
	}*/

}