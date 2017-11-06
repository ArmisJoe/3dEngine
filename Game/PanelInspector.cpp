#include "PanelInspector.h"
#include "Application.h"

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
	//ImGui::SetNextWindowPos(pos);
	ImGui::SetNextWindowContentSize(size);

	ImGui::Begin(name, &active, ImGuiWindowFlags_HorizontalScrollbar);

	if (inspected != nullptr)
		inspected->OnEditor();

	ImGui::End();
}

void PanelInspector::SetInspected(GameObject * new_insp)
{
	inspected = new_insp;
}
