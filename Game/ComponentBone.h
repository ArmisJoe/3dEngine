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
private:
	ComponentMesh* mesh = nullptr;
public:
	ComponentMesh* GetMesh() const;
	void SetMesh(ComponentMesh* m);
public:
	std::vector<ResourceBone*> skeleton;
};