#include "ModuleImGui.h"
#include "PanelConsole.h"

#include <iostream> 
#include <string>

#include <random>

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
			if (ImGui::Button("Go Random", ImVec2(100, 50))) {
				for (int i = 0; i < 100; i++) {
					random_device rd;
					mt19937 gen(rd());
					uniform_real_distribution<> dis(0.1, 1);
					float r = dis(gen);
					AddSphere(dis(gen), dis(gen), dis(gen), r);
				}

				console->ConsoleLog("All Spheres Created");
			}
			ImGui::End();
		}
	}

	if (ImGui::Begin("Collision Check")) {
		if (ImGui::Button("Check Collisions", ImVec2(100, 50))) {
			CheckAllIntersec();
		}
		ImGui::End();
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

Sphere* ModuleImGui::AddSphere(const float x, const float y, const float z, float radius)
{
	math::float3 point = { x, y, z };
	Sphere* s = new Sphere(point, radius);
	spheres.push_back(s);
	return s;
}

int ModuleImGui::CheckIntersec(Sphere * sp)
{
	int ret = 0;

	if (!spheres.empty()) {
		for (std::list<Sphere*>::iterator it = spheres.begin(); it != spheres.end(); it++) {
			if (it._Ptr->_Myval->Intersects(*sp)) {
				ret++;
			}
		}
	}

	return ret;
}

void ModuleImGui::CheckAllIntersec()
{
	int a_id = 1;
	for (std::list<Sphere*>::iterator it_a = spheres.begin(); it_a != spheres.end(); it_a++) {
		int b_id = 1;
		for (std::list<Sphere*>::iterator it_b = spheres.begin(); it_b != spheres.end(); it_b++) {
			if (it_a._Ptr->_Myval->Intersects(*(it_b._Ptr->_Myval))) {
				string collision_log = "Sphere " + std::to_string(a_id) + " collides with Sphere " + std::to_string(b_id);
				console->ConsoleLog(collision_log.c_str());
			}
			b_id++;
		}
		a_id++;
	}
}
