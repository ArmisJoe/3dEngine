#include "ModuleEditorUI.h"
#include "PanelConsole.h"
#include "PanelInspector.h"
#include "PanelAbout.h"
#include "PanelSceneTree.h"

#include <iostream> 
#include <string>
#include "Hardware.h"

ModuleEditorUI::ModuleEditorUI(Application * app, bool start_enabled) : Module(app, start_enabled)
{
	name = "GUI";

	show_test_window = true;
	config_active = false;
	object_p = false;
	//libraries.push_back("ImGui");
	//libraries.push_back("MathGeoLib");
	//libraries.push_back("gpudetect");
	//libraries.push_back("SDL");
	//libraries.push_back("Glew v2.1.0");
	//libraries.push_back("DevIL 1.8.0");
}

ModuleEditorUI::~ModuleEditorUI()
{
	show_test_window = true;
	object_p = false;
	config_active = false;
	console = nullptr;
	inspector = nullptr;
	about = nullptr;
	sceneTree = nullptr;
}

bool ModuleEditorUI::Init()
{

	ImGui_ImplSdl_Init(App->window->window);

	ImGuiIO& io = ImGui::GetIO();
	io.IniFilename = "Settings/imgui.ini";

	AddPanel(console = new PanelConsole());
	AddPanel(inspector = new PanelInspector());
	AddPanel(about = new PanelAbout());
	AddPanel(sceneTree = new PanelSceneTree());

	return true;
}

bool ModuleEditorUI::Start()
{
	console->active = true;
	inspector->active = true;
	about->active = false;

	sceneTree->SetRoot(App->scene->GetRoot());

	console->size = { (float)App->window->screen_surface->w * 2 / 3, (float)App->window->screen_surface->h / 4 };
	console->pos = { App->window->screen_surface->w/2 - console->size.x/2 , App->window->screen_surface->h - (console->size.y ) };

	//Versions
	std::string library_str;
	// ImGui
	library_str = "ImGui v";
	library_str.append(ImGui::GetVersion());
	libraries.push_back(library_str);
	//MathGeo
	libraries.push_back("MathGeoLib v1.5");
	//GpDetect
	libraries.push_back("gpudetect Apache 2.0 license");
	//SDL
	SDL_version ver;
	SDL_GetVersion(&ver);
	library_str = "SDL v";
	std::string tmp = "SDL v" + ver.minor + '.' + ver.patch + '.' + ver.major;
	library_str.append(tmp.c_str());
	tmp.clear();
	libraries.push_back(library_str);
	//OpenGL
	libraries.push_back("OpenGL v2.1.0");
	//Glew
	libraries.push_back("Glew v2.1.0");
	//DevIL
	libraries.push_back("DevIL v1.8.0");

	about->libraries = libraries;

	return true;
}

update_status ModuleEditorUI::PreUpdate(float dt)
{
	ImGui_ImplSdl_NewFrame(App->window->window);

	ImGuiIO& io = ImGui::GetIO();

	//Set Keyboard / Mouse bindings here

	return update_status::UPDATE_CONTINUE;
}

update_status ModuleEditorUI::Update(float dt)
{

	if (App->Logs.size() > 0)
	{
		console->ConsoleLog(App->Logs.c_str());
		App->Logs = "";
	}

	for (std::list<Panel*>::iterator it = panels.begin(); it != panels.end(); it++) {
		if ((*it)->active)
			(*it)->Draw();
	}

	if (ImGui::BeginMainMenuBar()) {
		if (ImGui::BeginMenu("File")) {
			ImGui::MenuItem("Config", "Alt+F1", &config_active);
			if (ImGui::Button("Save")) {
				JSON_Doc* config = App->parson->config;
				config->Save();
				LOG("Configuration Saved");
			}
			if (ImGui::Button("Quit")) {
				App->input->AppQuit(true);
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Objects")) {
			if (ImGui::BeginMenu ("Create")) {
				if (ImGui::Button("Sphere")) {
					App->physics->AddSphere(0, 0, 0, 0.4f);
				}
				if (ImGui::Button("AABB")) {
					App->physics->AddAABB(vec(0, 0, 0), vec(1, 1, 1));
				}
				ImGui::EndMenu();
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Windows")) {
			ImGui::MenuItem("Object Creation", "O", &object_p);
			ImGui::MenuItem("Scene Tree", "S", &sceneTree->active);
			ImGui::MenuItem("Inspector", "I", &inspector->active);
			ImGui::MenuItem("Console", "Alt+C", &console->active);
			ImGui::Separator();
			if(ImGui::Button("Clear Console")) {
				console->Clear();
			}
			ImGui::EndMenu();
		}
		if (ImGui::SmallButton("About")) {
			about->active = !about->active;
		}
		ImGui::EndMainMenuBar();
	}

	/// Test Window Code
	//show_test_window = false;
	//
	//if (show_test_window == true) {
	//	ImGui::ShowTestWindow(&show_test_window);
	//	ImGui::ShowMetricsWindow(&show_test_window);
	//}

	if (object_p) {
		if (ImGui::Begin("Object Creation")) {
			if (ImGui::Button("Go Random", ImVec2(100, 50))) {
				for (int i = 0; i < 100; i++) {
					float r = RandomNumber(0, 0.2f);
					App->physics->AddSphere(RandomNumber(0, 20), RandomNumber(0, 20), RandomNumber(0, 20), r);
				}

				LOG("All Spheres Created");
			}
			ImGui::End();
		}
	}

	/// Collision Check Button
	//if (ImGui::Begin("Collision Check")) {
	//	if (ImGui::Button("Check Collisions", ImVec2(100, 50))) {
	//		App->physics->CheckAllIntersec();
	//	}
	//	ImGui::End();
	//}

	if (config_active) {
		DrawConfigPanels();
	}

	return update_status::UPDATE_CONTINUE;
}

update_status ModuleEditorUI::PostUpdate(float dt)
{
	return update_status::UPDATE_CONTINUE;
}

bool ModuleEditorUI::CleanUp()
{
	for (std::list<Panel*>::iterator it = panels.begin(); it != panels.end(); it++) {
		if ((*it) != nullptr) {
			(*it)->CleanUp();
			delete (*it);
		}
	}

	panels.clear();

	console = nullptr;
	inspector = nullptr;

	return true;
}

void ModuleEditorUI::Draw()
{

	ImGui::Render();

}

void ModuleEditorUI::ConsoleLog(const char * str) const
{
	console->ConsoleLog(str);
}

void ModuleEditorUI::ClearLog()
{
	console->Clear();
}

void ModuleEditorUI::AddPanel(Panel * n_panel)
{
	panels.push_back(n_panel);
}


void ModuleEditorUI::DrawConfigPanels()
{

	ImGui::SetNextWindowContentSize(ImVec2(500, 500));

	if (ImGui::Begin("Configuration", &config_active, ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_AlwaysVerticalScrollbar)) {

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
				ShellExecute(NULL, "open", "https://github.com/ArmisJoe/3dEngine/issues", NULL, NULL, SW_SHOWMAXIMIZED);
			}
			if (ImGui::Button("Download latest version"))
			{
				ShellExecute(NULL, "open", "https://github.com/ArmisJoe/3dEngine/releases", NULL, NULL, SW_SHOWMAXIMIZED);
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

		//Render
		App->renderer3D->DrawConfigPanel();

		// TExtures
		App->tex->DrawConfigPanel();
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
