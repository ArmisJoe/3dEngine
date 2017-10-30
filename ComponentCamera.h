#pragma once

#include "Component.h"



class ComponentCamera : public Component {
public:
	ComponentCamera();
	ComponentCamera(GameObject* argparent);
	ComponentCamera(componentType argtype, GameObject* argparent);
	virtual ~ComponentCamera() {};
public:
	virtual void OnEditor();

	void Start();
	void Update();
	void CleanUp();

private:
	Frustum frustum;

};
