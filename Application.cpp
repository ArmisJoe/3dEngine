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
	//parson = new ModuleParson(this);

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
	//AddModule(parson);
	
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

	fps_container.push_back(ImGui::GetIO().Framerate);
	if (fps_container.size() > 500) {
		fps_container.erase(fps_container.begin(), fps_container.begin()+1);
	}

	/*ms_container.push_back(1000.f/ImGui::GetIO().Framerate);*/


	if (ms_container.size() > 500) {
		ms_container.erase(ms_container.begin(), ms_container.begin() + 1);
	}

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
	fps_container.erase(fps_container.begin(), fps_container.begin() + 500);
	for (list<Module*>::iterator it = list_modules.begin(); it != list_modules.end(); ++it) {
		if ((*it) != nullptr && ret == true) {
			ret = (*it)->CleanUp();
		}
	}
	return ret;
}

void Application::DrawConfigPanel()
{
	if (ImGui::CollapsingHeader("Application")) {
		ImGui::Text("Framerate");
		fps_title = EngineName +  ':' + std::to_string(ImGui::GetIO().Framerate);
		ImGui::PlotHistogram("Framerate", &fps_container[0], fps_container.size(), 0, fps_title.c_str(), 0.0f, 100.0f, { 500, 100 });
		ImGui::Separator();
		ms_title = EngineName + ':' + std::to_string(1000.f /ImGui::GetIO().Framerate);
		/*ImGui::PlotHistogram("Miliseconds", &ms_container[0], ms_container.size(), 0, ms_title.c_str(), 0.0f, 40.0f, { 500, 100 });*/
	}
}

void Application::AddModule(Module* mod)
{
	list_modules.push_back(mod);
}

void Application::mylog(const char file[], int line, const char* format, ...)
{
	static char tmp_string[4096];
	static char tmp_string2[4096];
	static va_list  ap;

	// Construct the string from variable arguments
	va_start(ap, format);
	vsprintf_s(tmp_string, 4096, format, ap);
	va_end(ap);
	sprintf_s(tmp_string2, 4096, "\n%s(%d) : %s", file, line, tmp_string);
	Logs += tmp_string2;
}