#pragma once

#include "Component.h"

class ComponentMesh : public Component {

	ComponentMesh() { };
	virtual ~ComponentMesh() {};

public:
	void DrawInspectorPanel();
public:
	//Vertices
	unsigned int id_vertices = 0; // id in VRAM
	unsigned int num_indices = 0;
	unsigned int* indices = nullptr;
	//Indices
	unsigned int id_indices = 0; // id in VRAM
	unsigned int num_vertices = 0;
	float* vertices = nullptr;
	//Triangles
	unsigned int num_triangles = 0; // A.K.A. num_faces
									//Texture
	Texture* tex = nullptr; // Mesh Texture
	unsigned int id_UV = 0; // id in VRAM
	float* textureCoords; // UV Coords
	unsigned int num_UV = 0;
	float material_index; // material ID
	unsigned int num_UVChannels = 0;
	//Scale
	vec3 scale = { 1, 1, 1 };
};