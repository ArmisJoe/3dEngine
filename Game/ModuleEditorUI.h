#pragma once

//#include "Module.h"
#include "Panel.h"

#include "Application.h"

#include "ImGui\imgui.h"
#include "ImGui\imconfig.h"
#include "ImGui\imgui_internal.h"

#include "imgui_impl_sdl.h"
#include "SDL/include/SDL_opengl.h"

#include "MathGeoLib\MathGeoLib.h"

class Module;
class Application;
class PanelConsole;
class PanelAbout;
class PanelInspector;
class PanelSceneTree;
class PanelSceneSave;
class PanelSceneLoad;

class ModuleEditorUI : public Module {
public:
	ModuleEditorUI(bool start_enabled = true);
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
	//bool AreGizmosActive() const { return GizmosActive; }
	//void GizmoStart() { GizmosActive = true; }
	//void GizmoEnd() { GizmosActive = false; }
private:
	std::list<Panel*> panels;

	bool object_p = false;

	bool config_active = false;
	bool show_test_window = true;

	// List of libraries which actually they are strings which they are char*-based classes themselves from C but also used on C# and (like on this case) C++
	vector<string> libraries;

	//bool GizmosActive = true;

public:
	PanelConsole*		console = nullptr;
	PanelInspector*		inspector = nullptr;
	PanelAbout*			about = nullptr;
	PanelSceneTree*		sceneTree = nullptr;
	PanelSceneSave*		savescene = nullptr;
	PanelSceneLoad*		loadscene = nullptr;
};
	