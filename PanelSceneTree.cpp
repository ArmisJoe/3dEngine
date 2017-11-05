#include "PanelSceneTree.h"

PanelSceneTree::PanelSceneTree()
{
	name = "SceneTree";
	size = { 250, 500 };
	pos = { 10, 10 };
}

PanelSceneTree::~PanelSceneTree()
{
}

void PanelSceneTree::Draw()
{
	ImGui::SetNextWindowContentSize(size);

	ImGui::Begin(name, &active, ImGuiWindowFlags_HorizontalScrollbar);

	if(root != nullptr && !root->children.empty())
		root->OnHierarchyTree(true);

	ImGui::End();
}
