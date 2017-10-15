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
class PanelAbout;
class PanelInspector;

class ModuleEditorUI : public Module {
public:
	ModuleEditorUI(Application* app, bool start_enabled = true);
	~ModuleEditorUI();

	bool Init();
	bool Start();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	void Draw();

	void ConsoleLog(const char* str) const;
	void ClearLog();

	void AddPanel(Panel* n_panel);

	void DrawConfigPanels();

private:
	std::list<Panel*> panels;

	PanelConsole* console = nullptr;
	PanelInspector* inspector = nullptr;

	bool object_p = false;

	bool config_active = false;
	bool show_test_window = true;

	// libraries
	vector<string> libraries;
};
	