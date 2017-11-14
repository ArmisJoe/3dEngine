#include "PanelSceneLoad.h"
#include "dirent.h"

#include "HelperFoos.h"

void PanelSceneLoad::Draw()
{

	std::vector<std::string> files;

	struct dirent* pent = NULL;
	DIR* pdir = NULL;
	if ((pdir = opendir(dir_path)) != 0) {
		while ((pent = readdir(pdir)) != NULL) {
			if(pent->d_type == DT_REG)
				files.push_back(std::string(pent->d_name));
		}
		closedir(pdir);
	}

	ImGui::Begin("Load Scene");
	ImGui::Text("Select Loading File");
	ImGui::Separator();
	for (int i = 0; i < files.size(); i++) {
		if (ImGui::Selectable(files[i].c_str()) && files[i].c_str() != nullptr) {
			std::strcpy(scene_name, files[i].c_str());
		}
	}
	ImGui::Separator();
	ImGui::Text("Scene:");
	ImGui::SameLine();
	if (scene_name == nullptr)
		strcpy(scene_name, "");
	ImGui::InputText("", scene_name, 50);
	if (ImGui::SmallButton("Load") && strcmp(scene_name, "")) {
		App->scene->LoadScene(scene_name, true);
		LOG("Scene Loaded '%s'", scene_name);
	}
	ImGui::SameLine();
	if (ImGui::SmallButton("Cancel")) {
		this->active = false;
	}
	ImGui::End();
}

void PanelSceneLoad::ChangeDir(const char * new_path)
{
	dir_path = new_path;
}
