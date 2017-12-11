
#include <iostream> 
#include <sstream> 

#include "Application.h"



Application::Application()
{
	window = new ModuleWindow();
	input = new ModuleInput();
	audio = new ModuleAudio();
	picker = new ModulePicker();
	scene = new ModuleScene();
	renderer3D = new ModuleRenderer3D();
	camera = new ModuleCamera3D();
	physics = new ModulePhysics3D();
	player = new ModulePlayer();
	editor = new ModuleEditorUI();
	parson = new ModuleParson();
	assimp = new ModuleAssimp();
	tex = new ModuleTextures();
	res = new ModuleResources();
	fs = new ModuleFileSystem();
	quadTree = new ModuleQuadtree();
	game = new ModuleGame();
	animation = new ModuleAnimationLoader();
	bone_loader = new ModuleBoneLoader();

	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(window);
	AddModule(camera);
	AddModule(input);
	AddModule(audio);
	AddModule(physics);
	AddModule(editor);
	AddModule(parson);
	AddModule(assimp);
	AddModule(tex);
	AddModule(res);
	AddModule(fs);
	AddModule(game);
	AddModule(animation);

	// Scenes
	AddModule(scene);
	AddModule(player);

	// Picker
	AddModule(quadTree);
	AddModule(picker);

	// Renderer last!
	AddModule(renderer3D);
}

Application::~Application()
{
	for (std::list<Module*>::reverse_iterator rit = list_modules.rbegin(); rit != list_modules.rend(); ++rit)
	{
		if ((*rit) != nullptr)
		mdelete *rit;
	}

	list_modules.clear();

}

bool Application::Init()
{
	start_up.Start();

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
	string temp = "Time for start up " + to_string(start_up.ReadMs()) + "ms";
	editor->ConsoleLog(temp.c_str());
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

	stats = m_getMemoryStatistics();

	mem_container.push_back((float)stats.totalReportedMemory);

	if (mem_container.size() > 500) {
		mem_container.erase(mem_container.begin(), mem_container.begin() + 1);
	}

	for (list<Module*>::iterator it = list_modules.begin(); it != list_modules.end(); ++it) {
		if ((*it) != nullptr && ret == true) {
			ret = (*it)->PreUpdate(dt);
		}
	}

	for (list<Module*>::iterator it = list_modules.begin(); it != list_modules.end(); ++it) {
		if ((*it) != nullptr && ret == true) {
			module_timer.Start();
			ret = (*it)->Update(dt);
			AddModulePerformance((*it)->name, module_timer.ReadMs());
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
	if (ImGui::CollapsingHeader("Application")) {
		ImGui::Text("Framerate");
		ImGui::Separator();

		string fps_title = std::to_string(ImGui::GetIO().Framerate);
		ImGui::PlotHistogram("Framerate", &fps_container[0], fps_container.size(), 0, fps_title.c_str(), 0.0f, 100.0f, { 500, 100 });
		ImGui::Separator();

		ms_title = EngineName + ':' + std::to_string(1000.f /ImGui::GetIO().Framerate);

		ImGui::Text("Memory Consumption");
		ImGui::Separator();

		string mem_title = std::to_string(mem_container.back());

		ImGui::PlotHistogram("Memory Consumtion", &mem_container[0], mem_container.size(), 0, mem_title.c_str(), 0.0f, stats.peakReportedMemory, { 500, 100 });

		int i = 0;
		if (!module_container.empty())
		{
			map<string, vector<float>>::iterator it;
			it = module_container.begin();
			for (; it != module_container.end(); ++it)
			{
				//sprintf("Modules Performance", "%.5fms", it->second[it->second.size() - 1]);
				if (i % 4 != 0) ImGui::SameLine();
				string mod_title = std::to_string(it->second[it->second.size() - 1]);
				ImGui::PlotHistogram(it->first.c_str(), &it->second[0], it->second.size(), 0, mod_title.c_str(), 0.0f, 10.0f, { 100, 50 });
				i++;
			}

			ImGui::Text("Total Reported Mem: %u", stats.totalReportedMemory);
			ImGui::Text("Total Actual Mem: %u", stats.totalActualMemory);
			ImGui::Text("Peak Reported Mem: %u", stats.peakReportedMemory);
			ImGui::Text("Peak Actual Mem: %u", stats.peakActualMemory);
			ImGui::Text("Accumulated Reported Mem: %u", stats.accumulatedReportedMemory);
			ImGui::Text("Accumulated Actual Mem: %u", stats.accumulatedActualMemory);
			ImGui::Text("Accumulated Alloc Unit Count: %u", stats.accumulatedAllocUnitCount);
			ImGui::Text("Total Alloc Unit Count: %u", stats.totalAllocUnitCount);
			ImGui::Text("Peak Alloc Unit Count: %u", stats.peakAllocUnitCount);
		}
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

void Application::AddModulePerformance(const string name, float ms)
{
	map<string, vector<float>>::iterator it;


	it = module_container.find(name);

	if (it != module_container.end())
	{
		if (it->second.size() == 100)
		{
			for (int i = 0; i < 100 - 1; i++)
			{
				it->second[i] = it->second[i + 1];
			}
			it->second[100 - 1] = ms;
		}
		else
			it->second.push_back(ms);
	}
	else
	{
		std::vector<float> temp;
		temp.push_back(ms);
		module_container.insert(pair<string, vector<float>>(name, temp));
	}

}