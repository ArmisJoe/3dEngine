#include "Application.h"
#include <iostream> 
#include <sstream> 


Application::Application()
{
	window = new ModuleWindow(this);
	input = new ModuleInput(this);
	audio = new ModuleAudio(this, true);
	scene_intro = new ModuleSceneIntro(this);
	renderer3D = new ModuleRenderer3D(this);
	camera = new ModuleCamera3D(this);
	physics = new ModulePhysics3D(this);
	player = new ModulePlayer(this);
	imgui = new ModuleImGui(this);

	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(window);
	AddModule(camera);
	AddModule(input);
	AddModule(audio);
	AddModule(physics);
	AddModule(imgui);
	
	// Scenes
	AddModule(scene_intro);
	AddModule(player);

	// Renderer last!
	AddModule(renderer3D);
}

Application::~Application()
{
	for (std::list<Module*>::reverse_iterator rit = list_modules.rbegin(); rit != list_modules.rend(); ++rit)
	{
		if ((*rit) != nullptr)
		delete *rit;
	}

	list_modules.clear();

}

bool Application::Init()
{
	bool ret = true;

	// Call Init() in all modules

	EngineName = "PJ Edu Engine";

	for (list<Module*>::iterator it = list_modules.begin(); it != list_modules.end(); ++it) {
		if ((*it) != nullptr && ret == true) {
			ret = (*it)->Init();
		}
	}
	

	// After all Init calls we call Start() in all modules
	//LOG("Application Start --------------");

	for (list<Module*>::iterator it = list_modules.begin(); it != list_modules.end(); ++it) {
		if ((*it) != nullptr && ret == true) {
			ret = (*it)->Start();
		}
	}
	ms_timer.Start();
	return ret;
}

// ---------------------------------------------
void Application::PrepareUpdate()
{
	dt = (float)ms_timer.Read() / 1000.0f;
	ms_timer.Start();
}

// ---------------------------------------------
void Application::FinishUpdate()
{
}

// Call PreUpdate, Update and PostUpdate on all modules
update_status Application::Update()
{
	update_status ret = UPDATE_CONTINUE;
	PrepareUpdate();
	
	for (list<Module*>::iterator it = list_modules.begin(); it != list_modules.end(); ++it) {
		if ((*it) != nullptr && ret == true) {
			ret = (*it)->PreUpdate(dt);
		}
	}

	for (list<Module*>::iterator it = list_modules.begin(); it != list_modules.end(); ++it) {
		if ((*it) != nullptr && ret == true) {
			ret = (*it)->Update(dt);
		}
	}

	for (list<Module*>::iterator it = list_modules.begin(); it != list_modules.end(); ++it) {
		if ((*it) != nullptr && ret == true) {
			ret = (*it)->PostUpdate(dt);
		}
	}
	FinishUpdate();
	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;
	for (list<Module*>::iterator it = list_modules.begin(); it != list_modules.end(); ++it) {
		if ((*it) != nullptr && ret == true) {
			ret = (*it)->CleanUp();
		}
	}
	return ret;
}

void Application::DrawConfigPanel()
{
	if (ImGui::BeginMenu("Application")) {

		ImGui::Text(EngineName);

		ImGui::EndMenu();
	}
}

void Application::AddModule(Module* mod)
{
	list_modules.push_back(mod);
}