#pragma once

#include "Component.h"

#include "glmath.h"
#include "src\MathGeoLib.h"

class ComponentTransform : public Component {
public:
	ComponentTransform();
	ComponentTransform(GameObject* argparent);
	ComponentTransform(componentType argtype, GameObject* argparent);
	~ComponentTransform() {};
public:
	//void Update(float dt);
	void OnEditor();
	float4x4 GetWorldMatrix() { UpdateMatrix(); return WorldMatrix; }
public:
	float3 position;
	Quat rotation;
	float3 scale;
	float4x4 WorldMatrix = float4x4::zero;
private:
	void UpdateMatrix();
};
