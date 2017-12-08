#pragma once

#include "Component.h"

class ComponentBone;

class ComponentMesh : public Component {
public:
	ComponentMesh();
	ComponentMesh(GameObject* argparent);
	ComponentMesh(componentType argtype, GameObject* argparent);
	virtual ~ComponentMesh() {};

public:
	void Start();
	void Update(float dt);
	void CleanUp();
	void Draw(const ComponentTransform* trans = nullptr, const ComponentMaterial* mat = nullptr);
	void OnEditor();
	void Serialize(JSON_Doc* doc);

	// Animation stuff
	void SetSkin();
	void DoSkin(GameObject* go);
	void ResetDeformableMesh();

public:
	//Path
	std::string path;
	std::string raw_path;
	//Vertices
	uint id_vertices = 0; // id in VRAM
	uint num_indices = 0;
	uint * indices = nullptr;
	//Indices
	uint id_indices = 0; // id in VRAM
	uint num_vertices = 0;
	float* vertices = nullptr;
	//Triangles
	uint GetTriangles() {
		return 2 - num_vertices + num_indices;
	}
	//UVs
	uint id_UV = 0; // id in VRAM
	float* textureCoords = nullptr; // UV Coords
	uint num_UV = 0;
	uint num_UVChannels = 0;

	float material_index = 0; // material ID

	bool visible = true;

	//Animation
	ComponentMesh* skin = nullptr;
	float* normals;
	uint id_normals;
	vector<ComponentBone*> bones;
};