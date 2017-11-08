#pragma once

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
	void SetScale(const float3& scale);

	float3 GetLocalPosition()const;
	Quat GetLocalRotation()const;
	float3 GetLocalScale()const;

	
	void SetTransformMatrix();
	const float* GetLocalTransform();
	int GetTransformID()const;

	float4x4 GetGlobalMatrix()const { return WorldMatrix; }
	float4x4 GetTransformMatrix();
	void SetIdentityTransform();
	void OnEditor();

	bool Transformed = false;
private:

	float3 position = float3::zero;
	Quat rotation = Quat::identity;
	float3 scale = float3::one;

	float4x4 transform_matrix;
	float4x4 prev_local_transform;

	float4x4 WorldMatrix;


	int transform_id;

	bool transform_modified = false;

	float4x4 view_matrix_fromCamera;
	float4x4 proj_matrix_fromCamera;
};
