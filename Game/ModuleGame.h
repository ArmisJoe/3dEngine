#pragma once
#ifndef __MODULE_GAME_H__
#define __MODULE_GAME_H__

#include "Module.h"

#define TIMEMULTIPLIER_LIMIT 3.0f
#define TIMEMULTIPLIER_STEP	1.0f

enum GameState {
	gameState_editor = 0,
	gameState_play,
	// Unknown always last
	gameState_unknown
};

class ModuleGame : public Module {
	friend class PanelPlayPause;
public:
	ModuleGame(bool start_enabled = true);
	~ModuleGame();
public:	// Module::
	bool Init();
	bool Start();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();
public: // Specific::
	update_status UpdateGame(float dt);
	update_status NextFrame();
public: // Utilities::
	float SetTimeMultiplier(float tM); // Returns the final tM after clamp.
	float GetTimeMultiplier() const {
		return timeMultiplier;
	}
	void SetGameState(uint gs);
	int GetGameState() const {
		return gameState;
	}
	void SetPaused(bool flag) {
		Paused = flag;
	}
	bool GetPaused() const {
		return Paused;
	}
private:
	uint gameState = gameState_editor;
	uint tmp_gamestate = gameState_editor;
	float timeMultiplier = 1.0f;
	float dTime = 0;
protected:
	bool Paused = false;
};

#endif // !__MODULE_GAME_H__
