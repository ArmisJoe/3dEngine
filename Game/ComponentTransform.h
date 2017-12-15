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

	void Serialize(JSON_Doc* doc);

	float4x4	GetTransform() const;
	float3		GetPosition() const;
	Quat		GetQuatRotation() const;
	float3		GetEulerRotation() const;
	float3		GetScale() const;
	float4x4	GetGlobalTransform() const;
	float4x4	GetGlobalTransformT() const;

private:
	void UpdateEulerAngles();
	void UpdateTransform();
public:

	void SetPosition(float3 position);
	void SetScale(float3 scale);
	void SetQuatRotation(Quat rotation);
	void SetEulerRotation(float3 euler_angles);
	void SetGlobalTransform(float4x4 transform);
	
	void OnUpdateTransform(const float4x4& global, const float4x4& parent_global = float4x4::identity);

	void LoadGlobalTransform(float4x4 transform);


private:
	float4x4	transform = float4x4::identity;
	float4x4	global_transform = float4x4::identity;

	float3		position = float3::zero;
	float3		scale = float3::zero;
	Quat		rotation = Quat::identity;
	float3		rotinEuler = float3::zero;

public:
	bool		transform_modified = true;
};