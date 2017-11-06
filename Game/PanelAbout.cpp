#include "PanelAbout.h"

#include "ModuleEditorUI.h"

void PanelAbout::Draw()
{
	ImGui::Begin("About");
	
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
	
	ImGui::End();

}
