#pragma once
 
#include "ImGui\imgui.h"
#include "ImGuizmo\ImGuizmo.h"
#include "Component.h"

#include "glmath.h"
#include "MathGeoLib\MathGeoLib.h"

class ComponentTransform : public Component {
public:

	ComponentTransform();
	ComponentTransform(GameObject* argparent);
	ComponentTransform(componentType argtype, GameObject* argparent);
	ComponentTransform(GameObject* argparent, float3 position, Quat rotation, float3 scale);
	~ComponentTransform() {};

	void Update(float dt);

	void SetPosition(const float3& position);
	void SetRotation(const float3& rotation);
	void SetRotation(const Quat& rotation);
	void SetScale(const float3& scale);

	void LoadPosition(const float3& position);
	void LoadRotation(const Quat& rotation);
	void LoadScale(const float3& scale);

	float3 GetLocalPosition();
	Quat GetLocalRotation();
	float3 GetLocalScale();


	void SetTransformMatrix();
	const float* GetGlobalTransformPtr();
	float4x4 GetGlobalTransformMatrix();
	float4x4 GetLocalTransformMatrix();

	int GetTransformID()const;

	void SetIdentityTransform();
	void OnEditor();
	void Serialize(JSON_Doc* doc);
	bool transform_modified = false;
	bool UpdateNeeded = false;
	void SetLocalTrans();

private:


	float3 position_global = float3::zero;
	Quat rotation_global = Quat::identity;
	float3 scale_global = float3::one;
	float3 rotinEuler_global = float3::zero;

	float3 localPos = float3::zero;
	Quat localRot = Quat::identity ;
	float3 localScale = float3::one;

	float4x4 transform_matrix;
	int transform_id;

	ImGuizmo::OPERATION mCurrentGuizmoOperation = ImGuizmo::OPERATION::ROTATE;
	ImGuizmo::MODE mCurrentGuizmoMode = ImGuizmo::MODE::WORLD;
};