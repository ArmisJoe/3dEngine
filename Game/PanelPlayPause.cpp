#include "PanelPlayPause.h"

void PanelPlayPause::Draw()
{
	ImGui::Begin("GameTimeManager", &active, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
	
	bool playbutton = (App->game->GetGameState() == gameState_play) ? true : false;
	ImGui::Checkbox("PLAY", &playbutton);
	if (App->game->GetGameState() != gameState_play && playbutton == true)
		App->game->SetGameState(gameState_play);
	if (App->game->GetGameState() != gameState_editor && playbutton == false)
		App->game->SetGameState(gameState_editor);
	ImGui::SameLine();
	ImGui::Checkbox("PAUSE", &App->game->Paused);
	ImGui::SameLine();
	if (ImGui::SmallButton("Next Frame")) {
		App->game->NextFrame();
	}
	ImGui::Separator();
	float tm = App->game->GetTimeMultiplier();
	if (ImGui::SliderFloat("timeMultiplier", &tm, 0, TIMEMULTIPLIER_LIMIT, "%.0f", TIMEMULTIPLIER_STEP)) {
		App->game->SetTimeMultiplier((int)tm);
	}
	ImGui::End();
}
