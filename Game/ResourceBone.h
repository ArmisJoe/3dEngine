#pragma once
#ifndef __RESOURCEBONE_H__
#define __RESOURCEBONE_H__

#include <string>
#include <vector>

#include "MathGeoLib\MathGeoLib.h"

class ResourceBone {
public:

	float3 position = float3::zero;
	// texture coordinates ??¿?¿?¿?¿?
	// Normal?¿?¿?
	uint num_weigths = 0;
	uint* indices = nullptr;
	float* weigths = nullptr;
	//ComponentMesh* mesh = nullptr;

	// The reason why we need this matrix is because 
	// the vertices are stored in the usual local space. 
	// This means that even without skeletal animation support 
	// our existing code base can load the model and render it correctly.
	float4x4 trans = float4x4::identity; // ?¿?¿? the website says I need this. EDIT: oh
};

// for later:
// http://ogldev.atspace.co.uk/www/tutorial38/tutorial38.html

#endif // !__ANIMATION_H__
#pragma once
