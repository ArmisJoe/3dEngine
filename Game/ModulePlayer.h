#pragma once
#include "Module.h"
#include "Globals.h"
#include "Time.h"


struct PhysVehicle3D;

class ModulePlayer : public Module
{
public:
	ModulePlayer(bool start_enabled = true);
	virtual ~ModulePlayer();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

public:
	Timer time;
};