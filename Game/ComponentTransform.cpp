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
	rotinEuler = RadToDeg(rotation.ToEulerXYX());
	scale = { 1, 1, 1 };
}

ComponentTransform::ComponentTransform(GameObject* argparent) : Component(componentType_Transform, argparent)
{
	name = "Transform";
	numMax = 1;
	position = { 0, 0, 0 };
	rotation = { 0, 0, 0, 1 };
	rotinEuler = RadToDeg(rotation.ToEulerXYX());
	scale = { 1, 1, 1 };
}

ComponentTransform::ComponentTransform(componentType argtype, GameObject * argparent) : Component(componentType_Transform, argparent)
{
	name = "Transform";
	numMax = 1;
	position = { 0, 0, 0 };
	rotation = { 0, 0, 0, 1 };
	rotinEuler = RadToDeg(rotation.ToEulerXYX());
	scale = { 1, 1, 1 };

}

ComponentTransform::ComponentTransform(GameObject * argparent, float3 position, Quat rotation, float3 scale) : Component(componentType_Transform, argparent)
{
	this->position = position; this->rotation = rotation; this->scale = scale;
	rotinEuler = RadToDeg(rotation.ToEulerXYX());
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

const float* ComponentTransform::GetLocalTransformTransposed()
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
		rotinEuler = RadToDeg(rotation.ToEulerXYX());
	}
}

void ComponentTransform::UpdateTransFromParent(GameObject * parent)
{
	if (parent != nullptr)
	{
		ComponentTransform* parentTrans = (ComponentTransform*)parent->GetTransform();
		if (parentTrans != nullptr)
		{
			GlobalTransMatrix = parentTrans->LocalTransMatrix * GlobalTransMatrix;
			float4x4 temp;
			GlobalTransMatrix.Decompose(position, temp, scale);
			rotinEuler = temp.ToEulerXYZ();

			rotation = Quat::FromEulerXYZ(DegToRad(rotinEuler.x), DegToRad(rotinEuler.y), DegToRad(rotinEuler.z));
			GlobalTransMatrix = float4x4::FromQuat(rotation);
			GlobalTransMatrix = float4x4::Scale(scale, float3(0, 0, 0)) * GlobalTransMatrix;
			GlobalTransMatrix.float4x4::SetTranslatePart(position.x, position.y, position.z);

			if (GetParent() != nullptr && GetParent()->GetParent() != nullptr)
			{
				ComponentTransform* parentTrans = (ComponentTransform*)GetParent()->GetParent()->GetTransform();
				if (parentTrans != nullptr)
				{
					GlobalTransMatrix = parentTrans->GetGlobalMatrix() * GlobalTransMatrix;
				}
			}
		}
	}
}

void ComponentTransform::SetLocalTrans(GameObject* parent)
{
	if (parent != nullptr)
	{
		ComponentTransform* parentTrans = (ComponentTransform*)parent->GetTransform();
		if (parentTrans != nullptr)
		{
			float3 localPos = position - parentTrans->position;
			Quat localRot = rotation * parentTrans->rotation.Conjugated();
			float3 localScale = scale.Mul(parentTrans->scale.Recip());

			GlobalTransMatrix = float4x4::FromQuat(localRot);
			LocalTransMatrix = float4x4::Scale(localScale, float3(0, 0, 0)) * GlobalTransMatrix;
			LocalTransMatrix.float4x4::SetTranslatePart(localPos.x, localPos.y, localPos.z);
		}
		else
		{
			LocalTransMatrix = GlobalTransMatrix;
		}
	}
	else
	{
		LocalTransMatrix = GlobalTransMatrix;
	}
}


void ComponentTransform::Update(float dt)
{
	/*if (/*UpdateNeeded == true ||*/// GlobalTransMatrix.Equals(float4x4::zero))
	//{
		/*GlobalTransMatrix = GetTransformMatrix();

		if (GetParent() != nullptr && GetParent()->GetParent() != nullptr)
		{
			ComponentTransform* parent_transform = (ComponentTransform*)GetParent()->GetParent()->GetTransform();
			if (parent_transform != nullptr) {
				GlobalTransMatrix = GlobalTransMatrix * parent_transform->GetGlobalMatrix();
			}
		}
		rotinEuler = RadToDeg(rotation.ToEulerXYX());
		UpdateNeeded = false;
	}*/

	/*
		if (GetParent()->GetParent() != nullptr) {
			if (!GetParent()->IsRoot())
			{
				ComponentTransform* parent_transform = (ComponentTransform*)GetParent()->GetParent()->GetTransform();

				WorldMatrix = WorldMatrix.FromTRS(position, rotation, scale);
				if(parent_transform != nullptr)
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
		}*/
}

void ComponentTransform::Start()
{
	/*GlobalTransMatrix = GetTransformMatrix();

	if (GetParent() != nullptr)
	{
		ComponentTransform* parent_transform = (ComponentTransform*)GetParent()->GetParent()->GetTransform();
		if (parent_transform != nullptr) {
			GlobalTransMatrix = GlobalTransMatrix * parent_transform->GetGlobalMatrix();
		}
	}
	rotinEuler = RadToDeg(rotation.ToEulerXYX());*/
}

void ComponentTransform::SetPosition(const float3 & _position)
{
	if (GetParent() != nullptr && !GetParent()->IsStatic())
	{
		position = _position;
		transform_modified = true;
		UpdateNeeded = true;
	}

}

void ComponentTransform::SetRotation(const float3& _rotation)
{
	if (GetParent() != nullptr && !GetParent()->IsStatic())
	{
		Quat mod = Quat::FromEulerXYZ(_rotation.x, _rotation.y, _rotation.z);
		rotation = mod;

		transform_modified = true;
		UpdateNeeded = true;
	}
}

void ComponentTransform::SetRotation(const Quat& _rotation)
{
	if (GetParent() != nullptr && !GetParent()->IsStatic())
	{
		rotation = _rotation;

		transform_modified = true;
		UpdateNeeded = true;
	}
}


void ComponentTransform::SetScale(const float3 & _scale)
{
	if (GetParent() != nullptr && !GetParent()->IsStatic())
	{
		scale = _scale;

		transform_modified = true;
		UpdateNeeded = true;
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
				UpdateNeeded = true;
			}
		}
		if (ImGui::DragFloat3("Rotation:", rot, 0.1f)) {
			if (GetParent() != nullptr && !GetParent()->IsStatic())
			{
				rotation = rotation.FromEulerXYZ(DegToRad(rot[0]), DegToRad(rot[1]), DegToRad(rot[2]));
				transform_modified = true;
				UpdateNeeded = true;
			}
		}

		if (ImGui::DragFloat3("Scale:", sca, 0.1f)) {
			if (GetParent() != nullptr && !GetParent()->IsStatic())
			{
				scale.x = sca[0];
				scale.y = sca[1];
				scale.z = sca[2];
				transform_modified = true;
				UpdateNeeded = true;
			}
		}
	
	// ImGuizmo
	if (App->input->GetKey(SDL_SCANCODE_LCTRL) == KEY_REPEAT && GetParent() != nullptr && !GetParent()->IsStatic())
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

		//float4x4 trs_matrix = GlobalTransMatrix.Transposed();
		float4x4 trs_matrix = GetTransformMatrix().Transposed();

		ImGuizmo::Manipulate(view_matrix1.ptr(), view_matrix.ptr(), mCurrentGuizmoOperation, mCurrentGuizmoMode, trs_matrix.ptr());

		if (ImGuizmo::IsUsing())
		{
			//App->camera->SetCameraActive(false);
			//float3 new_pos;
			float3 new_scale;
			//float3 new_rot;

			ImGuizmo::DecomposeMatrixToComponents(trs_matrix.ptr(), position.ptr(), rotinEuler.ptr() , scale.ptr());
			trs_matrix.Transpose();
			ImGuizmo::RecomposeMatrixFromComponents((float*)position.ptr(), (float*)rotinEuler.ptr(), (float*)scale.ptr(), trs_matrix.ptr());
			trs_matrix.Transpose();
		
			rotation = rotation.FromEulerXYZ(DegToRad(rotinEuler[0]), DegToRad(rotinEuler[1]), DegToRad(rotinEuler[2]));
		//	rotation = rotinEuler
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