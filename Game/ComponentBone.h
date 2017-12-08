#pragma once

#include "Component.h"

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
};