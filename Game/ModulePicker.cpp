#include "ModulePicker.h"

ModulePicker::ModulePicker(Application * app, bool start_enabled) : Module(app, start_enabled)
{
	name = "Picker";
}

ModulePicker::~ModulePicker()
{
}

bool ModulePicker::Start()
{
	return true;
}

update_status ModulePicker::Update(float dt)
{
	return UPDATE_CONTINUE;
}

bool ModulePicker::CleanUp()
{
	return true;
}

GameObject * ModulePicker::RayCast(LineSegment segment, float & dist)
{
	GameObject* ret = nullptr;

	return ret;
}
