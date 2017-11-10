#pragma once

#include "Module.h"
#include "Globals.h"
#include "glmath.h"
#include "GameObject.h"
#include "MathGeoLib/MathGeoLib.h"
#include "MathGeoLib/Geometry/QuadTree.h"
#include "QuadTreeh.h"

class ModuleQuadtree : public Module
{
public:
	ModuleQuadtree(bool start_enabled = true);
	~ModuleQuadtree();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

private:
	Quadtree quadtreeh; // I deadass created the quadtree class when there is one in mathgeolib ok
};