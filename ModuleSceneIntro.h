#pragma once
#include "Module.h"
#include "p2DynArray.h"
#include "Globals.h"
#include "Primitive.h"
#include "PhysBody3D.h"
#include "ModulePlayer.h"

struct PhysBody3D;
struct PhysMotor3D;
struct PhysVehicle3D;

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();
	bool Start();
	update_status Update(float dt);
	bool CleanUp();
	//void ModuleSceneIntro::OnCollision(PhysBody3D * body1, PhysBody3D * body2);
public:
};
