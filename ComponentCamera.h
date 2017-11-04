#pragma once

#include "Component.h"

class ComponentCamera : public Component {
public:
	ComponentCamera();
	ComponentCamera(GameObject* argparent);
	ComponentCamera(componentType argtype, GameObject* argparent);
	virtual ~ComponentCamera();
public:
	virtual void OnEditor();

	void Start();
	void Update();
	void CleanUp();


	void SetPos(const float3 pos) { frustum->pos = pos; }
	float* GetViewMatrix() const;
	float* GetProjectionMatrix() const;
	void SetAspectRatio(const float &w, const float&h);
	void SetAspectRatio(const float & aspect_ratio);
	float GetFOV() const { return FOV; }
	float3 GetPos() const { return frustum->pos; }
	void GetCorners(float3* corners) { frustum->GetCornerPoints(corners); }
	Frustum* GetFrustum() const { return frustum; }
	void MoveUp(const float &movement);
	void MoveDown(const float &movement);
	void MoveLeft(const float &movement);
	void MoveRight(const float &movement);
	void MoveForwards(const float &movement);
	void MoveBackwards(const float &movement);
	void Rotate(const float& mov_y, const float& mov_z);

private:
	void UpdateProjection();
	bool SetFov(float argFOV);

	Frustum* frustum;
	float FOV;
	float aspect_ratio;
	//Color background_color = Black;
};
