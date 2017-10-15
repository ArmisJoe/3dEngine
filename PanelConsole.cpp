#include "PanelConsole.h"

PanelConsole::PanelConsole()
{
	name = "Console";
	size = { 500, 250 };
	pos = { 50, 300 };
}

PanelConsole::~PanelConsole()
{
}

void PanelConsole::Draw()
{
	ImGui::SetNextWindowPos(pos);
	ImGui::SetNextWindowContentSize(size);
	ImGui::Begin(name, &active, ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
	if(ImGui::SmallButton("Clear")) {
		Clear();
	}
	ImGui::Separator();

	ImGui::SetNextWindowContentSize(size);
	ImGui::BeginChild("ScrollingRegion", ImVec2(0, -ImGui::GetItemsLineHeightWithSpacing()), false, ImGuiWindowFlags_HorizontalScrollbar);

	ImGui::TextUnformatted(text_buffer.begin());

	if (scrollToBottom) {
		ImGui::SetScrollHere(1.0f);
		scrollToBottom = false;
	}

	ImGui::EndChild();

	ImGui::End();
}

void PanelConsole::Clear()
{
	text_buffer.clear();
}

void PanelConsole::ConsoleLog(const char * log)
{
	text_buffer.append(log);
	scrollToBottom = true;
}
