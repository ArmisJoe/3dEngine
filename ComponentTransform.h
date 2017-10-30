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
	void OnEditor();
public:
	float* GetMatrix4x4() const;
	float3 GetGlobalPos() const;
	Quat GetGlobalRot() const;
	float3 GetGlobalScale() const;
public:
	float3 position;
	Quat rotation;
	float3 scale;
private:
	float4x4 Matrix4x4;
};
