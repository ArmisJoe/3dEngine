#pragma once

//#include "Module.h"
#include "Panel.h"

#include "Application.h"

#include "ImGui\imgui.h"
#include "ImGui\imconfig.h"
#include "ImGui\imgui_internal.h"

#include "imgui_impl_sdl.h"
#include "SDL/include/SDL_opengl.h"

#include "src\MathGeoLib.h"

class Module;
class Application;
class PanelConsole;

class ModuleImGui : public Module {
public:
	ModuleImGui(Application* app, bool start_enabled = true);
	~ModuleImGui();

	bool Init();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	void AddPanel(Panel* n_panel);
	Sphere* AddSphere(const float x, const float y, const float z, float radius);

	int CheckIntersec(Sphere* sp);
	void CheckAllIntersec();

private:

	void UpdateAboutWin();


	std::list<Panel*> panels;

	PanelConsole* console = nullptr;

	bool object_p = false;
	bool about_p = false;
	bool hello2 = false;
	std::list<Sphere*> spheres;

	bool show_test_window = true;
};
	