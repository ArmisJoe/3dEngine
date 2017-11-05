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


	float* GetViewMatrix();

	void FocusMesh(const float *vertices, const uint &num_vertices);
	void LookAt(const float3 &spot);
	void DrawConfigPanel();
private:
	void RotateCamera(float dt);

	void MoveCamera(float dt);
	void CameraZoom(float dt);

public:
	ComponentCamera* curr_camera;
	ComponentCamera* main_camera;
	float3 orbitate_around = float3::zero;
};