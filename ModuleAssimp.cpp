#include "ModuleAssimp.h"
#include "Application.h"
#include "ModuleCamera3D.h"

#include "ComponentMaterial.h"
#include "ComponentMesh.h"
#include "ComponentTransform.h"

ModuleAssimp::ModuleAssimp(Application * app, bool start_enabled) : Module(app, start_enabled)
{
	name = "Assimp";
}

ModuleAssimp::~ModuleAssimp()
{
}

GameObject * ModuleAssimp::LoadNode(const aiNode * node, const aiScene* scene)
{
	assert(node != nullptr);
	assert(scene != nullptr);

	GameObject* new_node = new GameObject();

	aiMaterial** materials = nullptr;
	if (scene->HasMaterials())
		materials = scene->mMaterials;
	else
		LOG("Scene without materials");

	//LoadMeshes
	for (uint i = 0; i < node->mNumMeshes; i++) {
		//Mesh Load
		ComponentMesh* new_mesh = LoadMesh(scene->mMeshes[node->mMeshes[i]]);
		if(new_mesh != nullptr)
			new_node->AddComponent(componentType_Mesh, new_mesh);
		//Material Load
		if (materials != nullptr && new_mesh != nullptr) {
			ComponentMaterial* new_material = LoadMaterial(materials[(int)new_mesh->material_index]);
			if (new_material != nullptr) {
				new_node->AddComponent(componentType_Material, new_material);
			}
		}
	}

	//Transform
	if (!new_node->FindComponents(componentType_Transform).empty()) {
		ComponentTransform* ref_transform = (ComponentTransform*)new_node->FindComponents(componentType_Transform)[0];
		if (ref_transform != nullptr) {
			aiVector3D translation, scaling;
			aiQuaternion rotation;
			node->mTransformation.Decompose(scaling, rotation, translation);
			ref_transform->position = { translation.x, translation.y, translation.y };
			ref_transform->rotation = { rotation.x, rotation.y, rotation.z, rotation.w };
			ref_transform->scale = { scaling.x, scaling.y, scaling.z };
		}
	}
		

	//Node Children 'Recursivity'
	for (uint i = 0; i < node->mNumChildren; i++) {
		new_node->children.push_back(LoadNode(node->mChildren[i], scene));
	}

	if(new_node != nullptr)
		App->res->gameObjects.push_back(new_node);

	return new_node;
}

GameObject* ModuleAssimp::LoadGeometry(const char* path, const unsigned int pprocess_flag)
{

	App->editor->ClearLog();

	GameObject* Geometry = nullptr;

	const aiScene* scene = aiImportFile(path, pprocess_flag);
	const aiNode* root_node = nullptr;

	if (scene != nullptr)
	{
		//ROOT Node
		root_node = scene->mRootNode;
		//Loading All nodes into Root Node
		Geometry = LoadNode(root_node, scene);
		//Camera Focus
		//App->camera->FocusMesh(new_mesh->vertices, new_mesh->num_vertices);
		//Release Scene
		aiReleaseImport(scene);
	}
	else
		LOG("Error loading scene %s", path);

	return Geometry;

}

ComponentMesh * ModuleAssimp::LoadMesh(const aiMesh* m)
{
	ComponentMesh* new_mesh = nullptr;
	
	//Vertices
	new_mesh = new ComponentMesh();
	new_mesh->material_index = m->mMaterialIndex;
	new_mesh->num_triangles = m->mNumFaces;
	new_mesh->num_vertices = m->mNumVertices;
	new_mesh->vertices = new float[new_mesh->num_vertices * 3];
	memcpy(new_mesh->vertices, m->mVertices, sizeof(float) * new_mesh->num_vertices * 3);
	glGenBuffers(1, (GLuint*) &(new_mesh->id_vertices));
	glBindBuffer(GL_ARRAY_BUFFER, new_mesh->id_vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * new_mesh->num_vertices * 3, new_mesh->vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//Indices
	if (m->HasFaces()) {
		new_mesh->num_indices = m->mNumFaces * 3;
		new_mesh->indices = new uint[new_mesh->num_indices];

		for (uint i = 0; i < m->mNumFaces; ++i)
		{
			if (m->mFaces[i].mNumIndices != 3) {
				LOG("Mesh face with != 3 indices!");
			}
			else
				memcpy(&new_mesh->indices[i * 3], m->mFaces[i].mIndices, 3 * sizeof(uint));
		}

	}
	else {
		LOG("Mesh with no Faces");
	}

	glGenBuffers(1, (GLuint*) &(new_mesh->id_indices));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, new_mesh->id_indices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * new_mesh->num_indices, new_mesh->indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	//UVS
	if (m->HasTextureCoords(0))
	{
		new_mesh->num_UV = m->mNumVertices;
		new_mesh->textureCoords = new float[new_mesh->num_UV * 3];
		memcpy(new_mesh->textureCoords, m->mTextureCoords[0], sizeof(float)*new_mesh->num_UV * 3);

		glGenBuffers(1, (GLuint*)&(new_mesh->id_UV));
		glBindBuffer(GL_ARRAY_BUFFER, (GLuint)new_mesh->id_UV);
		glBufferData(GL_ARRAY_BUFFER, sizeof(uint) * new_mesh->num_UV * 3, new_mesh->textureCoords, GL_STATIC_DRAW);
	}
	else
	{
		LOG("No Texture Coords found");
	}
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//Name
	new_mesh->name = m->mName.C_Str();

	LOG("Loaded mesh with %d vertices %d indices %d UVs", new_mesh->num_vertices, new_mesh->num_indices, new_mesh->num_UV);

	App->res->meshes.push_back(new_mesh);

	return new_mesh;
}

ComponentMaterial * ModuleAssimp::LoadMaterial(const aiMaterial* mat)
{
	ComponentMaterial* new_mat = nullptr;
	new_mat = new ComponentMaterial();

	// Diffuse
	if (mat->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
		for (uint i = 0; i < mat->GetTextureCount(aiTextureType_DIFFUSE); i++) {
			aiString m_path;
			mat->GetTexture(aiTextureType_DIFFUSE, i, &m_path);
			if (m_path.length > 0) {
				new_mat->SetTextureChannel(texType_Diffuse, App->tex->LoadTexture(m_path.C_Str()));
			}
			else {
				LOG("Unvalid Path from texture: %s", m_path.C_Str());
			}
		}
	}
	else {
		LOG("No Diffuse found");
	}
	//All other texture types...

	if (new_mat->HasTextures()) {
		App->res->materials.push_back(new_mat);
	}
	else {
		new_mat->CleanUp();
		delete[] new_mat;
		new_mat = nullptr;
	}

	return new_mat;
}

bool ModuleAssimp::CleanUp()
{
	return true;
}