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
	if (GetParent() != nullptr && !GetParent()->IsStatic())
	{
		SetPosition({ 0,0,0 });
		Quat rot;
		SetRotation({ 0.0f, 0.0f, 0.0f });
		SetScale({ 1,1,1 });
	}
}

void ComponentTransform::Update(float dt)
{
	if (can_update)
	{
		if (GetParent() != nullptr) {
			if (!GetParent()->IsRoot())
			{
				ComponentTransform* parent_transform = (ComponentTransform*)GetParent()->GetParent()->GetTransform();

				WorldMatrix = WorldMatrix.FromTRS(position, rotation, scale);
				if (parent_transform != nullptr)
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
	}
}

void ComponentTransform::SetPosition(const float3 & _position)
{
	if (GetParent() != nullptr && !GetParent()->IsStatic())
	{
		position = _position;
		transform_modified = true;
	}

}

void ComponentTransform::SetRotation(const float3& _rotation)
{
	if (GetParent() != nullptr && !GetParent()->IsStatic())
	{
		Quat mod = Quat::FromEulerXYZ(_rotation.x, _rotation.y, _rotation.z);
		rotation = mod;

		transform_modified = true;

	}
}

void ComponentTransform::SetRotation(const Quat& _rotation)
{
	if (GetParent() != nullptr && !GetParent()->IsStatic())
	{
		rotation = _rotation;

		transform_modified = true;

	}
}

void ComponentTransform::LoadPosition(const float3& argposition)
{
	position = argposition;
	transform_modified = true;
}
void ComponentTransform::LoadRotation(const Quat& argrotation)
{
	rotation = argrotation;
	transform_modified = true;
}
void ComponentTransform::LoadScale(const float3& argscale)
{
	scale = argscale;
	transform_modified = true;
}

void ComponentTransform::SetScale(const float3 & _scale)
{
	if (GetParent() != nullptr && !GetParent()->IsStatic())
	{
		scale = _scale;

		transform_modified = true;
	}
}


void ComponentTransform::OnEditor()
{
	ImGui::TextColored(COLOR_YELLOW, "Transform:");
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
		if (GetParent() != nullptr && !GetParent()->IsStatic())
		{
			position.x = pos[0];
			position.y = pos[1];
			position.z = pos[2];
			transform_modified = true;
		}
	}
	if (ImGui::DragFloat3("Rotation:", rot, 0.1f)) {
		if (GetParent() != nullptr && !GetParent()->IsStatic())
		{
			rotation = rotation.FromEulerXYZ(DegToRad(rot[0]), DegToRad(rot[1]), DegToRad(rot[2]));
			transform_modified = true;
		}
	}

	if (ImGui::DragFloat3("Scale:", sca, 0.1f)) {
		if (GetParent() != nullptr && !GetParent()->IsStatic())
		{
			scale.x = sca[0];
			scale.y = sca[1];
			scale.z = sca[2];
			transform_modified = true;
		}
	}

	// ImGuizmo
	if (App->input->GetKey(SDL_SCANCODE_LCTRL) == KEY_REPEAT & GetParent() != nullptr && !GetParent()->IsStatic())
	{
		App->camera->SetCameraActive(false);
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

		if (ImGui::Button("Translate") || App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
			mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
		ImGui::SameLine();
		if (ImGui::Button("Rotate") || App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
			mCurrentGizmoOperation = ImGuizmo::ROTATE;
		ImGui::SameLine();
		if (ImGui::Button("Scale") || App->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN)
			mCurrentGizmoOperation = ImGuizmo::SCALE;


		ImGuiIO& io = ImGui::GetIO();


		ImGuizmo::SetRect(0, 0, io.DisplaySize.x + 2, io.DisplaySize.y + 2);


		float4x4 view_matrix = App->camera->curr_camera->GetFrustum().ViewMatrix();
		float4x4 proj_matrix = App->camera->curr_camera->GetFrustum().ProjectionMatrix();
		view_matrix.Transpose();
		proj_matrix.Transpose();

		float4x4 trs_matrix = GetTransformMatrix();


		ImGuizmo::Manipulate(view_matrix.ptr(), proj_matrix.ptr(), mCurrentGizmoOperation, mCurrentGizmoMode, trs_matrix.ptr());

		if (ImGuizmo::IsUsing())
		{

			trs_matrix.Transpose();
			float3 new_pos = position;
			float3 new_scale = scale;
			Quat new_q = rotation;
			trs_matrix.Decompose(new_pos, new_q, new_scale);

			if (mCurrentGizmoOperation == ImGuizmo::TRANSLATE)
				SetPosition(float3(new_pos.x, new_pos.y, new_pos.z));
			if (mCurrentGizmoOperation == ImGuizmo::SCALE)
				SetScale(float3(new_scale.x, new_scale.y, new_scale.z));
			if (mCurrentGizmoOperation == ImGuizmo::ROTATE)
			{
				//new_q.T
				//rotation = rotation.FromEulerXYZ(DegToRad(new_q.ToEulerXYZ()[0]), DegToRad(new_q.ToEulerXYZ()[1]), DegToRad(new_q.ToEulerXYZ()[2]));
			}
			transform_modified = true;
		}
	}
	else {
		ImGuizmo::Enable(false);
		App->camera->SetCameraActive(true);
	}
}

float4x4 ComponentTransform::GetParentTransform() const
{
	/*GameObject* g = GetParent()->GetParent();
	float4x4 parent_transform = float4x4::identity;
	if (g != nullptr && g->IsRoot() == false)
	{
	if (g->GetTransform() != nullptr)
	parent_transform = g->GetTransform()->GetTransformMatrix();
	return parent_transform;
	}
	else return transform_matrix;*/
	return float4x4::zero;
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

	transform_modified = true;
}