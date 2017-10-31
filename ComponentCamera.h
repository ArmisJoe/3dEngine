#pragma once

#include "Component.h"


class Camera
{
public:
	Camera::Camera();
	Camera::~Camera();

public:
	bool SetFov(float argFOV);
	void MoveUp(const float &movement);
	void MoveDown(const float &movement);
	void MoveLeft(const float &movement);
	void MoveRight(const float &movement);
	void MoveForwards(const float &movement);
	void MoveBackwards(const float &movement);
	void Rotate(const float& mov_y, const float& mov_z);

	void SetPos(const float3 pos) { frustum.pos = pos; }
	void SetAspectRatio(const float &w, const float&h);
	float GetFOV() const { return FOV; }
	float3 GetPos() const { return frustum.pos; }

public:
	void GetCorners(float3* corners) { frustum.GetCornerPoints(corners); }
	Frustum frustum;
	float FOV;
	float aspect_ratio;
};


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
	Camera* camera;
};
