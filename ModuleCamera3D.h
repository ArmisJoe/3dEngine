#pragma once
#include "Module.h"
#include "Globals.h"
#include "glmath.h"


#define PROPORTION_CAMERA 1.5;


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

	float GetFOV() const { return FOV; }
private:
	Frustum frustum;
	float FOV;
	float aspect_ratio;
};


class ModuleCamera3D : public Module
{
public:
	ModuleCamera3D(Application* app, bool start_enabled = true);
	~ModuleCamera3D();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void Look(const vec3 &Position, const vec3 &Reference, bool RotateAroundReference = false);
	void LookAt(const vec3 &Spot);
	void Move(const vec3 &Movement);
	float* GetViewMatrix();

	void FocusMesh(const float *vertices, const uint &num_vertices);

private:

	void CalculateViewMatrix();

	void RotateCamera();
	void RotateFromReference();

	void MoveCamera(float dt);
	void CameraZoom(float dt);

	void SetCamera(const vec3& focus, const float& distance);

public:
	
	vec3 X, Y, Z, Position, Reference;

private:
	Camera* main_camera;

	mat4x4 ViewMatrix, ViewMatrixInverse;
};