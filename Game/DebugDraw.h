#pragma once

#include "MathGeoLib\MathGeoLib.h"

#include "Color.h"

class DebugDraw {
public:
	DebugDraw();
	~DebugDraw();
public:
	void SetLinesColor(Color argcolor) { color = argcolor; }
	void SetLinesSize(float argsize);	
private:
	void Draw(float3 * line_points, int size, float3 rgba = float3(255, 255, 255), float a = 1) const;
	Color color = White;
	float line_size = 1.0f;
public:
	void DrawFrustum(float3* vertices);
	void DrawAABB(float3 center, float3 size, float3 rgb = float3(255, 255, 255), float a = 1);
	void Switch();
	bool active = true;
public:
	void SetColor(Color argcol);
};