#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "Primitive.h"
#include "PhysVehicle3D.h"
#include "PhysBody3D.h"
#include "Timer.h"
#include "ModuleAudio.h"



ModulePlayer::ModulePlayer( bool start_enabled) : Module( start_enabled)
{
	name = "Player";
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");
	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");
	return true;
}


// Update: draw background
update_status ModulePlayer::Update(float dt)
{
	/*float camera_x = vehicle->vehicle->getChassisWorldTransform().getOrigin().getX();
	float camera_z = vehicle->vehicle->getChassisWorldTransform().getOrigin().getZ() + 15;
	float camera_y = vehicle->vehicle->getChassisWorldTransform().getOrigin().getY() + 10;*/
	/*App->camera->LookAt(vec3(camera_x, camera_y, camera_z));
	App->camera->Position = pos;*/

	return UPDATE_CONTINUE;
}

