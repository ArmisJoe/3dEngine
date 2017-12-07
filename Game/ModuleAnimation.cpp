#include "ModuleAnimation.h"


ModuleAnimation::ModuleAnimation(bool start_enabled) : Module(start_enabled)
{
}


ModuleAnimation::~ModuleAnimation()
{
}

bool ModuleAnimation::Init()
{
	return true;
}

bool ModuleAnimation::Start()
{
	return true;
}

update_status ModuleAnimation::PreUpdate()
{
	return UPDATE_CONTINUE;
}

update_status ModuleAnimation::Update()
{
	return UPDATE_CONTINUE;
}

update_status ModuleAnimation::PostUpdate()
{
	return UPDATE_CONTINUE;
}

bool ModuleAnimation::CleanUp()
{
	return true;
}
