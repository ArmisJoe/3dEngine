#pragma once

#include <list>
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include "SDL\include\SDL.h"


#include "Globals.h"

using namespace std;

class Application;
struct PhysBody3D;

class Module
{
private :
	bool enabled;
protected:
	bool menu_open = false;

public:
	char* name = "Module";

	Module(bool start_enabled = true)
	{}

	virtual ~Module()
	{}

	virtual bool Init() 
	{
		return true; 
	}

	virtual bool Start()
	{
		return true;
	}

	virtual update_status PreUpdate(float dt)
	{
		return UPDATE_CONTINUE;
	}

	virtual update_status Update(float dt)
	{
		return UPDATE_CONTINUE;
	}

	virtual update_status PostUpdate(float dt)
	{
		return update_status::UPDATE_CONTINUE;
	}

	virtual bool CleanUp() 
	{ 
		return true; 
	}

	virtual void Draw() {
		
	}

	virtual void OnCollision(PhysBody3D* body1, PhysBody3D* body2)
	{}

	// Config Panels
	virtual void DrawConfigPanel() 
	{}
	// Config Load [JSON]
	virtual void LoadConfig() 
	{}

};