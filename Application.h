#pragma once


#include "Globals.h"
#include "Timer.h"
#include "Module.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModuleSceneIntro.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ModulePhysics3D.h"
#include "ModulePlayer.h"
#include "ModuleImGui.h"

class ModuleImGui;

class Application
{
public:
	ModuleWindow* window;
	ModuleInput* input;
	ModuleAudio* audio;
	ModuleSceneIntro* scene_intro;
	ModuleRenderer3D* renderer3D;
	ModuleCamera3D* camera;
	ModulePhysics3D* physics;
	ModulePlayer* player;
	ModuleImGui* imgui;

private:

	Timer	ms_timer;
	float	dt;
	list<Module*> list_modules;

	char* EngineName;

public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

	void DrawConfigPanel();

private:

	vector<float> fps_container;
	vector<float> ms_container;
	string fps_title;
	string ms_title;

	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();
};