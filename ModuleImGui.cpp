#include "ModuleImGui.h"
#include "PanelConsole.h"

ModuleImGui::ModuleImGui(Application * app, bool start_enabled) : Module(app, start_enabled)
{
	show_test_window = true;
	object_p = false;
}

ModuleImGui::~ModuleImGui()
{
	show_test_window = true;
	object_p = false;
	console = nullptr;
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

	if (ImGui::BeginMainMenuBar()) {
		if (ImGui::BeginMenu("File")) {
			if (ImGui::Button("Quit")) {
				return update_status::UPDATE_STOP;
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Objects")) {
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("View")) {
			ImGui::MenuItem("Object Creation", "O", &object_p);
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

	if (object_p) {
		if (ImGui::Begin("Object Creation")) {
			if (ImGui::Button("Go Random", ImVec2(50, 100))) {
				vec3 centre;
				for (int i = 0; i < 100; i++) {
					centre.x = 1;
					centre.y = 1;
					centre.z = 1;
					float r = 10;
					//AddSphere(centre, r);
					ImGuiTextBuffer t;
					t.append("Created Sphere nº ");
					t.append("%s", i + 1);
					console->ConsoleLog(t.begin());
				}

			}
			ImGui::End();
		}
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

	if (!spheres.empty()) {
		for (std::list<Sphere*>::iterator it = spheres.begin(); it != spheres.end(); it++) {
			if (it._Ptr->_Myval != nullptr) {
				delete it._Ptr->_Myval;
			}
		}
	}

	console = nullptr;

	return true;
}

void ModuleImGui::AddPanel(Panel * n_panel)
{
	panels.push_back(n_panel);
}

Sphere* ModuleImGui::AddSphere(const vec3 &center, float radius)
{
	Sphere* s = nullptr;
	s = new Sphere();
	s->r = radius;
	//s->pos = { center.x, center.y, center.z };
	//spheres.push_back(s);
	return s;
}
