#pragma once

#include "Module.h"
#include "Globals.h"
#include "glmath.h"
#include "GameObject.h"


class ModulePicker : public Module
{
public:
	ModulePicker(bool start_enabled = true);
	~ModulePicker();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	GameObject* RayCast(const LineSegment &segment, float& total_distance);
	void IterativeRayCast(const LineSegment& segment, float &dist, GameObject** tocollide);
	GameObject* GetPicked() const { return picked; }
	bool TestObject(GameObject* go) { return (go == picked); }

private:
	GameObject* picked = nullptr;
	const float infinite = FLOAT_INF;
};