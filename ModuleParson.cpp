#include "ModuleParson.h"

ModuleParson::ModuleParson(Application * app, bool start_enabled) : Module(app, start_enabled)
{
	name = "Parson";
}

ModuleParson::~ModuleParson()
{
}


bool ModuleParson::Init()
{
	return true;
}

bool ModuleParson::CleanUp()
{
	return true;
}
