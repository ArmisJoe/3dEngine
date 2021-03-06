#pragma once
#include "Module.h"
#include "Globals.h"
#include "glmath.h"
#include "ComponentCamera.h"

#define PROPORTION_CAMERA 1.5;

class ModuleCamera3D : public Module
{
public:
	ModuleCamera3D(bool start_enabled = true);
	~ModuleCamera3D();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();


	float* GetViewMatrix();
	float* GetProjectionMatrix();


	void FocusMesh(const float *vertices, const uint &num_vertices);
	void LookAt(const float3 &spot);
	void DrawConfigPanel();

	void SetCameraActive(bool set) { active_camera = set; }
	bool IsActive() const { return active_camera; }

	void LoadConfig();

private:
	bool active_camera = true;
	void RotateCamera(float dt);
	void MoveCamera(float dt);
	void CameraZoom(float dt);
public:

	ComponentCamera* curr_camera;
	ComponentCamera* main_camera;
	float3 orbitate_around = float3::zero;
};