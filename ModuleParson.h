#pragma once

#ifndef __MODULE_PARSON_H__
#define __MODULE_PARSON_H__

#include "Module.h"
//#include "Globals.h"


class ModuleParson : public Module {
public:
	ModuleParson(Application* app, bool start_enabled = true);
	~ModuleParson();

public:
	bool Init();
	bool CleanUp();
};

#endif // !__MODULE_PARSON_H__
