#pragma once
#ifndef __MODULE_ANIMATION_H__
#define __MODULE_ANIMATION_H__

#include "Module.h"

class ModuleAnimation : public Module {
public:
	ModuleAnimation(bool start_enabled = true);
	~ModuleAnimation();
public:	// Module::
	bool Init();
	bool Start();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();
};

#endif // MODULE_ANIMATION