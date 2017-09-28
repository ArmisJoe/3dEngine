#include "PanelConsole.h"

PanelConsole::PanelConsole()
{
	w = 500;
	h = 250;
}

PanelConsole::~PanelConsole()
{
}

void PanelConsole::Draw()
{
	ImGui::SetNextWindowContentSize(ImVec2(w, h));
	ImGui::Begin("Console", &active, ImGuiWindowFlags_NoFocusOnAppearing);
	ImGui::TextUnformatted(text_buffer.begin());
	ImGui::End();
}

void PanelConsole::Clear()
{
	text_buffer.clear();
}

void PanelConsole::ConsoleLog(const char * log)
{
	text_buffer.append(log);
	text_buffer.append("\n");
}
