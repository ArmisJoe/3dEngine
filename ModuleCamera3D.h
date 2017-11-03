#pragma once
#include "Module.h"
#include "Globals.h"
#include "glmath.h"
#include "ComponentCamera.h"

#define PROPORTION_CAMERA 1.5;

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
	ComponentCamera* curr_camera;

private:
	
	vec3 X, Y, Z, Position, Reference;
	ComponentCamera* main_camera;

private:
	mat4x4 ViewMatrix, ViewMatrixInverse;
};