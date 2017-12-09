#pragma once

#include "Component.h"
#include "ComponentMesh.h"
#include "ResourceBone.h"

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
	ResourceBone* bone = nullptr;
private:
	ComponentMesh* mesh = nullptr;
};