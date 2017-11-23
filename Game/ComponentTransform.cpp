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
}

ComponentTransform::ComponentTransform(GameObject* argparent) : Component(componentType_Transform, argparent)
{
	name = "Transform";
	numMax = 1;
	position_global = { 0, 0, 0 };
	rotation_global = { 0, 0, 0, 1 };
	scale_global = { 1, 1, 1 };
	rotinEuler_global = RadToDeg(rotation_global.ToEulerXYX());
}

ComponentTransform::ComponentTransform(componentType argtype, GameObject * argparent) : Component(componentType_Transform, argparent)
{
	name = "Transform";
	numMax = 1;
	position_global = { 0, 0, 0 };
	rotation_global = { 0, 0, 0, 1 };
	scale_global = { 1, 1, 1 };
	rotinEuler_global = RadToDeg(rotation_global.ToEulerXYX());
}

ComponentTransform::ComponentTransform(GameObject * argparent, float3 position, Quat rotation, float3 scale) : Component(componentType_Transform, argparent)
{
	this->position_global = position; this->rotation_global = rotation; this->scale_global = scale;
	rotinEuler_global = RadToDeg(rotation_global.ToEulerXYX());
}

float3 ComponentTransform::GetLocalPosition()const
{
	return position_global;
}

Quat ComponentTransform::GetLocalRotation()const
{
	return rotation_global;
}

float3 ComponentTransform::GetLocalScale()const
{
	return scale_global;
}

void ComponentTransform::SetTransformMatrix()
{

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
	/*if (can_update)
	{
		if (GetParent() != nullptr) {
			if (!GetParent()->IsRoot())
			{
				ComponentTransform* parent_transform = (ComponentTransform*)GetParent()->GetParent()->GetTransform();

				WorldMatrix = WorldMatrix.FromTRS(position_global, rotation_global, scale);
				if (parent_transform != nullptr)
					WorldMatrix = parent_transform->WorldMatrix * WorldMatrix;
			}
			else
			{
				WorldMatrix = float4x4::FromTRS(position_global, rotation_global, scale);
				for (std::vector<GameObject*>::iterator it = GetParent()->children.begin(); it != GetParent()->children.end(); ++it)
				{
					ComponentTransform* child_transform = (ComponentTransform*)(*it)->GetTransform();
					child_transform->Update(dt);
				}
			}
		}
	}*/
}
void ComponentTransform::SetLocalTrans(GameObject* parent)
 {
	if (parent != nullptr)
		 {
		ComponentTransform* parentTrans = (ComponentTransform*)parent->GetTransform();
		if (parentTrans != nullptr)
			 {
			float3 localPos = position_global - parentTrans->position_global;
			Quat localRot = rotation_global * parentTrans->rotation_global.Conjugated();
			float3 localScale = scale_global.Mul(parentTrans->scale_global.Recip());
			}
		}	
	}


void ComponentTransform::SetPosition(const float3 & _position)
{
	if (GetParent() != nullptr && !GetParent()->IsStatic())
	{
		position_global = _position;
		transform_modified = true;

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

	}
}

void ComponentTransform::SetRotation(const Quat& _rotation)
{
	if (GetParent() != nullptr && !GetParent()->IsStatic())
	{
		rotation_global = _rotation;

		transform_modified = true;

	}
}

void ComponentTransform::LoadPosition(const float3& argposition)
{
	position_global = argposition;
	transform_modified = true;
}
void ComponentTransform::LoadRotation(const Quat& argrotation)
{
	rotation_global = argrotation;
	rotinEuler_global = RadToDeg(rotation_global.ToEulerXYX());
	transform_modified = true;
}
void ComponentTransform::LoadScale(const float3& argscale)
{
	scale_global = argscale;
	transform_modified = true;
}

void ComponentTransform::SetScale(const float3 & _scale)
{
	if (GetParent() != nullptr && !GetParent()->IsStatic())
	{
		scale_global = _scale;

		transform_modified = true;
	}
}


void ComponentTransform::OnEditor()
{
	ImGui::TextColored(COLOR_YELLOW, "Transform:");
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
		}
	}
	if (ImGui::DragFloat3("Rotation:", rot, 0.1f)) {
		if (GetParent() != nullptr && !GetParent()->IsStatic())
		{
			rotation_global = rotation_global.FromEulerXYZ(DegToRad(rot[0]), DegToRad(rot[1]), DegToRad(rot[2]));
			rotinEuler_global = RadToDeg(rotation_global.ToEulerXYX());
			transform_modified = true;
		}
	}

	if (ImGui::DragFloat3("Scale:", sca, 0.1f)) {
		if (GetParent() != nullptr && !GetParent()->IsStatic())
		{
			scale_global.x = sca[0];
			scale_global.y = sca[1];
			scale_global.z = sca[2];
			transform_modified = true;
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

		//float4x4 trs_matrix = GlobalTransMatrix.Transposed();
		float4x4 trs_matrix = GetGlobalTransformMatrix().Transposed();

		ImGuizmo::Manipulate(view_matrix1.ptr(), view_matrix.ptr(), mCurrentGuizmoOperation, mCurrentGuizmoMode, trs_matrix.ptr());

		if (ImGuizmo::IsUsing())
		{
			//App->camera->SetCameraActive(false);
			//float3 new_pos;
			//float3 new_scale;
			//float3 new_rot;



			ImGuizmo::DecomposeMatrixToComponents(trs_matrix.ptr(), position_global.ptr(), (float*) RadToDeg(rotation_global.ToEulerXYZ()).ptr(), scale_global.ptr());
			trs_matrix.Transpose();
			ImGuizmo::RecomposeMatrixFromComponents((float*)position_global.ptr(), RadToDeg(rotation_global.ToEulerXYZ()).ptr(), (float*)scale_global.ptr(), trs_matrix.ptr());
			trs_matrix.Transpose();


			//rotation_global = rotation_global.FromEulerXYZ(DegToRad(rotinEuler_global[0]), DegToRad(rotinEuler_global[1]), DegToRad(rotinEuler_global[2]));
			//	rotation = rotinEuler
			transform_modified = true;
			//UpdateNeeded = true;
		}

		/*App->camera->SetCameraActive(false);
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

		float4x4 trs_matrix = GetGlobalTransformMatrix();


		ImGuizmo::Manipulate(view_matrix.ptr(), proj_matrix.ptr(), mCurrentGizmoOperation, mCurrentGizmoMode, trs_matrix.ptr());

		if (ImGuizmo::IsUsing())
		{

			trs_matrix.Transpose();
			float3 new_pos = position_global;
			float3 new_scale = scale_global;
			Quat new_q = rotation_global;
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
			transform_modified = true;*/
		//}
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