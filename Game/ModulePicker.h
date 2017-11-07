#pragma once

#include "Module.h"
#include "Globals.h"
#include "glmath.h"


class ModulePicker : public Module
{
public:
	ModulePicker(Application* app, bool start_enabled = true);
	~ModulePicker();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();
};