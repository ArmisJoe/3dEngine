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

	for (std::list<GameObject*>::iterator it = scene->gameObjects.begin(); it != scene->gameObjects.end(); it++) {
		GameObject* go = (*it);
		go->DrawInspectorPanel();	
	}

	ImGui::End();
}
