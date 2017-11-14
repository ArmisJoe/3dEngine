#include "PanelSceneSave.h"
#include <string>
#include "Application.h"

#include "dirent.h"
#include "HelperFoos.h"

void PanelSceneSave::Draw()
{

	std::vector<std::string> files;

	struct dirent* pent = NULL;
	DIR* pdir = NULL;
	if ((pdir = opendir(dir_path)) != 0) {
		while ((pent = readdir(pdir)) != NULL) {
			if (pent->d_type == DT_REG)
				files.push_back(std::string(pent->d_name));
		}
		closedir(pdir);
	}

	ImGui::Begin("Save Scene");
	ImGui::Text("Write a name to save the file");
	ImGui::Separator();
	for (int i = 0; i < files.size(); i++) {
		if (ImGui::Selectable(files[i].c_str()) && files[i].c_str() != nullptr) {
			std::strcpy(scene_name, files[i].c_str());
		}
	}
	ImGui::Separator();
	ImGui::Text("Scene:");
	ImGui::SameLine();
	ImGui::InputText("Scene Name", scene_name, 50);

	char* savebuttonlabel = "Save";
	for (int i = 0; i < files.size(); i++) {
		if (strcmp(scene_name, files[i].c_str()) == 0)
			savebuttonlabel = "Overwrite";	
	}

	if (ImGui::SmallButton(savebuttonlabel)) {
		App->scene->Serialize(scene_name);
		LOG("Scene Saved as '%s'", scene_name);
		std::strcpy(scene_name, "");
		this->active = false;
	}
	ImGui::SameLine();
	if (ImGui::SmallButton("Cancel")) {
		this->active = false;
	}
	ImGui::End();
}

void PanelSceneSave::SetSceneName(const char * str)
{
	strcpy(scene_name, str);
}

void PanelSceneSave::ChangeDir(const char * new_path)
{
	dir_path = new_path;
}
