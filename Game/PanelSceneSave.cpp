#include "PanelSceneSave.h"
#include <string>
#include "Application.h"
void PanelSceneSave::Draw()
{
	ImGui::Begin("Save Scene");
	ImGui::Text("Write a name to save the file");
	ImGui::Separator();
	ImGui::InputText("Scene Name", scene_name, 50);
	if (ImGui::SmallButton("Save")) {
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
