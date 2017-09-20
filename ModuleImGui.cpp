#include "ModuleImGui.h"
#include "PanelConsole.h"

ModuleImGui::ModuleImGui(Application * app, bool start_enabled) : Module(app, start_enabled)
{
	show_test_window = true;
}

ModuleImGui::~ModuleImGui()
{
	show_test_window = true;
}

bool ModuleImGui::Init()
{
	
	ImGui_ImplSdl_Init(App->window->window);

	ImGuiIO& io = ImGui::GetIO();
	io.IniFilename = "Settings/imgui.ini";

	AddPanel(console = new PanelConsole());

	return true;
}

update_status ModuleImGui::PreUpdate(float dt)
{
	ImGui_ImplSdl_NewFrame(App->window->window);

	console->ConsoleLog("PUTA");

	ImGuiIO& io = ImGui::GetIO();

	//Set Keyboard / Mouse bindings here

	return update_status::UPDATE_CONTINUE;
}

update_status ModuleImGui::Update(float dt)
{

	for (std::list<Panel*>::iterator it = panels.begin(); it != panels.end(); it++) {
		if((*it)->active)
			(*it)->Draw();
	}

	if (ImGui::Begin("Close Window")) {
		if (ImGui::Button("Quit", ImVec2(100, 100))) {
			return update_status::UPDATE_STOP;
		}
		ImGui::End();
	}

	if (ImGui::BeginMainMenuBar()) {
		if (ImGui::BeginMenu("View")) {
			ImGui::MenuItem("Console", "1", &console->active);
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}

	show_test_window = true;

	if (show_test_window == true) {
		ImGui::ShowTestWindow(&show_test_window);
		ImGui::ShowMetricsWindow(&show_test_window);
	}

	ImGui::Render();

	return update_status::UPDATE_CONTINUE;
}

update_status ModuleImGui::PostUpdate(float dt)
{
	return update_status::UPDATE_CONTINUE;
}

bool ModuleImGui::CleanUp()
{
	for (std::list<Panel*>::iterator it = panels.begin(); it != panels.end(); it++) {
		if ((*it) != nullptr) {
			delete (*it);
		}
	}

	panels.clear();

	console = nullptr;

	return true;
}

void ModuleImGui::AddPanel(Panel * n_panel)
{
	panels.push_back(n_panel);
}
