#include "PanelInspector.h"

PanelInspector::PanelInspector()
{
	name = "Inspector";
	size = { 250, 500 };
	pos = { 10, 10 };
}

PanelInspector::~PanelInspector()
{
}

void PanelInspector::Draw()
{
	ImGui::SetNextWindowPos(pos);
	ImGui::SetNextWindowContentSize(size);
	ImGui::Begin(name, &active, ImGuiWindowFlags_AlwaysUseWindowPadding);

	

	ImGui::End();
}
