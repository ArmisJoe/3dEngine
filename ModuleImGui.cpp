#include "ModuleImGui.h"
#include "PanelConsole.h"

#include <iostream> 
#include <string>

#include <random>
#include "Hardware.h"

ModuleImGui::ModuleImGui(Application * app, bool start_enabled) : Module(app, start_enabled)
{
	name = "GUI";

	show_test_window = true;
	config_active = false;
	object_p = false;
	libraries.push_back("ImGui");
	libraries.push_back("MathGeoLib");
	libraries.push_back("gpudetect");
}

ModuleImGui::~ModuleImGui()
{
	show_test_window = true;
	object_p = false;
	config_active = false;
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
			ImGui::MenuItem("Config", "Alt+F1", &config_active);
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

	if (config_active) {
		DrawConfigPanels();
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
			(*it)->CleanUp();
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

void ModuleImGui::DrawConfigPanels()
{

	ImGui::SetNextWindowContentSize(ImVec2(500, 500));

	if (ImGui::Begin("Configuration", &config_active, ImGuiWindowFlags_NoFocusOnAppearing)) {

		if (ImGui::CollapsingHeader("About")) {
			ImGui::Text("3D Engine by UPC students Pau and Josep");

			ImGui::Separator();

			if (ImGui::Button("Github repository"))
			{
				ShellExecute(NULL, "open", "https://github.com/ArmisJoe/3dEngine", NULL, NULL, SW_SHOWMAXIMIZED);
			}

			if (ImGui::Button("Read documentation"))
			{
				ShellExecute(NULL, "open", "https://github.com/ArmisJoe/3dEngine/wiki", NULL, NULL, SW_SHOWMAXIMIZED);
			}
			if (ImGui::Button("Report Issue"))
			{
			
			}
			ImGui::Text("Libraries:\n");
			for (int i = 0; i < libraries.size(); ++i) {
				ImGui::Text(libraries[i].c_str());
			}
			ImGui::Text("License: MIT");
		}

		App->DrawConfigPanel();
		
		//Window
		App->window->DrawConfigPanel();

		//Input
		App->input->DrawConfigPanel();

		//Hardware
		//App->hardware->DrawConfigPanel(

		if (ImGui::CollapsingHeader("Hardware")) {

			Hardware hw;
			Hardware::hardware_info info;
			info = hw.GetInfo();
			ImGui::Text("SDL Version:", info.sdl_version);
			ImGui::SameLine();
			ImGui::Text("%u", info.sdl_version);
			ImGui::Separator();

			ImGui::Text("CPUs:", "%u (Cache: %ukb)", info.cpu_count, info.l1_cachekb);
			ImGui::SameLine();
			ImGui::Text("%u", info.cpu_count, info.l1_cachekb);

			ImGui::Text("System RAM:");
			ImGui::SameLine();
			ImGui::Text("%.1fGb", info.ram_gb);

			ImGui::Separator();
			ImGui::Text("GPU:");
			ImGui::SameLine();
			ImGui::Text("vendor %u device %u", info.gpu_vendor, info.gpu_device);

			ImGui::Text("Brand:");
			ImGui::SameLine();
			ImGui::Text(info.gpu_brand);

			ImGui::Text("VRAM Budget:");
			ImGui::SameLine();
			ImGui::Text("%.1f Mb", info.vram_mb_budget);

			ImGui::Text("VRAM Usage:");
			ImGui::SameLine();
			ImGui::Text("%.1f Mb", info.vram_mb_usage);

			ImGui::Text("VRAM Available:");
			ImGui::SameLine();
			ImGui::Text("%.1f Mb", info.vram_mb_available);

			ImGui::Text("VRAM Reserved:");
			ImGui::SameLine();
			ImGui::Text("%.1f Mb", info.vram_mb_reserved);

		}


		ImGui::End();
	}
}
