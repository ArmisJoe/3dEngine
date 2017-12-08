#pragma once

#include "Component.h"
#include "ComponentMesh.h"

class ComponentBone : public Component {
public:
	ComponentBone();
	ComponentBone(GameObject* argparent);
	ComponentBone(componentType argtype, GameObject* argparent);
	virtual ~ComponentBone();

public:
	void Start();
	void Update(float dt);
	void CleanUp();

	void Enable();
	void Disable();

	void OnEditor();
public:
	ComponentMesh* GetMesh() const { return mesh; }
public:
	float4x4 Matrix = float4x4::identity;
private:
	ComponentMesh* mesh = nullptr;
};