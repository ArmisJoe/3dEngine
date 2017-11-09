#pragma once


#include "Globals.h"
#include "Timer.h"
#include "PerfTimer.h"
#include "Module.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModuleScene.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ModulePhysics3D.h"
#include "ModulePlayer.h"
#include "ModuleEditorUI.h"
#include "mmgr\mmgr.h"
#include "ModuleParson.h"
#include "ModuleAssimp.h"
#include "ModuleTextures.h"
#include "ModuleResources.h"
#include "ModuleFileSystem.h"

#define LOG(format, ...) App->mylog(__FILE__, __LINE__, format, __VA_ARGS__);

class ModuleEditorUI;

class Application
{
public:
	ModuleWindow* window;
	ModuleInput* input;
	ModuleAudio* audio;
	ModuleScene* scene;
	ModuleRenderer3D* renderer3D;
	ModuleCamera3D* camera;
	ModulePhysics3D* physics;
	ModulePlayer* player;
	ModuleEditorUI* editor;
	ModuleParson* parson;
	ModuleAssimp* assimp;
	ModuleTextures* tex;
	ModuleResources* res;
	ModuleFileSystem* fs;
	
private:

	Timer	ms_timer;
	float	dt;
	list<Module*> list_modules;
	PerfTimer module_timer;
	PerfTimer start_up;
	char* EngineName;

public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

	void DrawConfigPanel();
	
	string Logs;
	void mylog(const char file[], int line, const char* format, ...);
	void AddModulePerformance(const string name, float ms);

private:
	vector<float> fps_container;
	map<string, vector<float>> module_container;
	vector<float> mem_container;
	sMStats stats;

	
	string ms_title;


	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();
};

extern Application * App;