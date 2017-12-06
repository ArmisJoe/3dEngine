#include "ModuleAssimp.h"
#include "Application.h"
#include "ModuleCamera3D.h"

#include "ComponentMaterial.h"
#include "ComponentMesh.h"
#include "ComponentTransform.h"

#include "ModuleFileSystem.h"

#include "HelperFoos.h"
#include "ModuleQuadtree.h"



ModuleAssimp::ModuleAssimp(bool start_enabled) : Module(start_enabled)
{
	name = "Geometry Loader";
}

ModuleAssimp::~ModuleAssimp()
{
}

// NOT THE GOOD ONE
GameObject * ModuleAssimp::LoadNode(const aiNode * node, const aiScene* scene, GameObject* parent)
{
	assert(node != nullptr);
	assert(scene != nullptr);

	GameObject* new_node = new GameObject();

	if (parent != nullptr)
	{
		new_node->SetParent(parent);
		//new_node->GetParent()->children.push_back(new_node);
	}
	else {
		new_node->SetParent(App->scene->GetRoot());
	}

	aiMaterial** materials = nullptr;
	if (scene->HasMaterials())
		materials = scene->mMaterials;
	else
		LOG("Scene without materials");
	
	//Transform

		aiVector3D translation, scaling;
		aiQuaternion rotation(1, 0, 0, 0);
		node->mTransformation.Decompose(scaling, rotation, translation);
		float3 position = { 0, 0, 0 };
		float3 scale = { 1, 1, 1 };
		position = { translation.x, translation.y, translation.y };
		Quat rotation2 = Quat(rotation.x, rotation.y, rotation.z, rotation.w);
		scale = { scaling.x, scaling.y, scaling.z };
		ComponentTransform* trans = new ComponentTransform(new_node, position, rotation2, scale);
		vector<Component*> tmp = new_node->FindComponents(componentType_Transform);
		if (!tmp.empty())
			new_node->DestroyComponent(tmp[0]);
		new_node->AddComponent(componentType_Transform, trans, false);


	//LoadMeshes
		std::string mesh_path;
		ComponentMesh* new_mesh = nullptr;
	for (uint i = 0; i < node->mNumMeshes; i++) {
		//Mesh Load
		new_mesh = LoadToOwnFormat(scene->mMeshes[node->mMeshes[i]], mesh_path);
		if (new_mesh != nullptr) {
			new_mesh->path = mesh_path;
			new_node->AddComponent(componentType_Mesh, new_mesh);
		}
		//Material Load
		if (materials != nullptr && new_mesh != nullptr) {
			ComponentMaterial* new_material = LoadMaterial(materials[(int)new_mesh->material_index]);
			if (new_material != nullptr) {
				new_node->AddComponent(componentType_Material, new_material);
			}
		}
	}


	if (node->mName.length > 0)
		new_node->SetName(node->mName.C_Str());


	//Node Children 'Recursivity'
	for (uint i = 0; i < node->mNumChildren; i++) {
		new_node->AddChild(LoadNode(node->mChildren[i], scene, new_node));
	}


	if (new_node != nullptr)
	{
		App->res->gameObjects.push_back(new_node);
		new_node->SetStatic(true);
		App->quadTree->InsertObject(new_node);
	}
	return new_node;
}

// bingo
GameObject * ModuleAssimp::LoadNode(const aiNode * node, const aiScene* scene, const char* raw_path, GameObject* parent)
{
	assert(node != nullptr);
	assert(scene != nullptr);

	GameObject* new_node = new GameObject();

	if (parent != nullptr)
	{
		new_node->SetParent(parent);
		//new_node->GetParent()->children.push_back(new_node);
	}
	else {
		new_node->SetParent(App->scene->GetRoot());
	}

	aiMaterial** materials = nullptr;
	if (scene->HasMaterials())
		materials = scene->mMaterials;
	else
		LOG("Scene without materials");

	//Transform

	aiVector3D translation, scaling;
	aiQuaternion rotation(1, 0, 0, 0);
	node->mTransformation.Decompose(scaling, rotation, translation);
	float3 position = { 0, 0, 0 };
	float3 scale = { 1, 1, 1 };
	position = { translation.x, translation.y, translation.z };
	Quat rotation2 = Quat(rotation.x, rotation.y, rotation.z, rotation.w);
	scale = { scaling.x, scaling.y, scaling.z };
	ComponentTransform* trans = new ComponentTransform(new_node, position, rotation2, scale);
	vector<Component*> tmp = new_node->FindComponents(componentType_Transform);
	if (!tmp.empty())
		new_node->DestroyComponent(tmp[0]);
	new_node->AddComponent(componentType_Transform, trans, true);


	//LoadMeshes
	std::string mesh_path;
	ComponentMesh* new_mesh = nullptr;
	for (uint i = 0; i < node->mNumMeshes; i++) {
		//Mesh Load
		new_mesh = LoadToOwnFormat(scene->mMeshes[node->mMeshes[i]], mesh_path);
		if (new_mesh != nullptr) {
			new_mesh->path = mesh_path;
			new_mesh->raw_path = raw_path;
			new_node->AddComponent(componentType_Mesh, new_mesh);
		}
		//Material Load
		if (materials != nullptr && new_mesh != nullptr) {
			ComponentMaterial* new_material = LoadMaterial(materials[(int)scene->mMeshes[node->mMeshes[i]]->mMaterialIndex]);
			if (new_material != nullptr) {
				new_node->AddComponent(componentType_Material, new_material);
			}
		}
	}


	if (node->mName.length > 0)
		new_node->SetName(node->mName.C_Str());


	//Node Children 'Recursivity'
	for (uint i = 0; i < node->mNumChildren; i++) {
		new_node->AddChild(LoadNode(node->mChildren[i], scene, raw_path, new_node));
	}

	if (new_node != nullptr)
	{
		App->res->gameObjects.push_back(new_node);
		new_node->SetStatic(true);
		App->quadTree->InsertObject(new_node);
	}

	return new_node;
}

// nice
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
		if (root_node->mNumChildren > 0) {
			Geometry = LoadNode(root_node, scene, path);
		}
		//Camera Focus
		//App->camera->FocusMesh(new_mesh->vertices, new_mesh->num_vertices);
		//Release Scene
		aiReleaseImport(scene);
	}
	else
		LOG("Error loading scene %s", path);

	//if (Geometry != nullptr)
	//	App->res->gameObjects.push_back(Geometry);

	return Geometry;

}

ComponentMesh * ModuleAssimp::LoadRawMesh(const aiMesh* m)
{
	ComponentMesh* new_mesh = nullptr;
	
	new_mesh = new ComponentMesh();
	new_mesh->material_index = m->mMaterialIndex;
	new_mesh->num_vertices = m->mNumVertices;
	//Vertices
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
				string fullPath = "Assets\\";
				fullPath.append(m_path.C_Str());
				std::string tex_path;
				Texture* tmp_tex = App->tex->LoadToDDS(fullPath.c_str(), tex_path);
				if (tmp_tex != nullptr) {
					tmp_tex->path = tex_path.c_str();
					if (new_mat != nullptr)
						new_mat->SetTextureChannel(texType_Diffuse, tmp_tex);
				}
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

	if (new_mat != nullptr && new_mat->HasTextures()) {
		App->res->materials.push_back(new_mat);
	}
	else {
		if (new_mat != nullptr) {
			new_mat->CleanUp();
			mdelete[] new_mat;
			new_mat = nullptr;
		}
	}

	return new_mat;
}

bool ModuleAssimp::Import(const aiMesh * m, std::string & output_file)
{
	bool ret = false;

	if (m == nullptr) {
		LOG("ERROR Loading mesh -> Mesh is NULLPTR");
		return false;
	}

	if (m->HasFaces()) {
		for (int i = 0; i < m->mNumFaces; i++) {
			if (m->mFaces[i].mNumIndices != 3) {
				return false;
				LOG("ERROR Mesh with Face with != 3 indices -- I DON'T PLAY WITH DEMONS");
			}
		}
	}

	ComponentMesh* new_mesh = nullptr;

	new_mesh = new ComponentMesh();
	//Nums
	new_mesh->material_index = m->mMaterialIndex; 
	new_mesh->num_vertices = m->mNumVertices;
	//Vertices
	new_mesh->vertices = new float[new_mesh->num_vertices * 3];
	memcpy(new_mesh->vertices, m->mVertices, sizeof(float) * new_mesh->num_vertices * 3);

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

	//UVS
	if (m->HasTextureCoords(0))
	{
		new_mesh->num_UV = m->mNumVertices;
		new_mesh->textureCoords = new float[new_mesh->num_UV * 3];
		memcpy(new_mesh->textureCoords, m->mTextureCoords[0], sizeof(float)*new_mesh->num_UV * 3);
	}
	else
	{
		LOG("No Texture Coords found");
	}

	//Name
	new_mesh->name = m->mName.C_Str();

	if (new_mesh != nullptr) {
		ret = SaveToOwnFormat(new_mesh, output_file);
		new_mesh->CleanUp(); 
		mdelete new_mesh;
	}

	return ret;
}

bool ModuleAssimp::SaveToOwnFormat(ComponentMesh * m, std::string & output_file)
{
	bool ret = false;

	int bufferIdx[NUM_MESH_IDX] = { // Setting the buffer Indexes
		m->num_vertices,
		m->num_indices,
		m->num_UV
	};
	
	int bufferSize = 0;	// Setting the Buffer full size to allocate;
	bufferSize += sizeof(bufferIdx);
	bufferSize += sizeof(float) * m->num_vertices * 3;
	bufferSize += sizeof(unsigned int) * m->num_indices;
	bufferSize += sizeof(float) * m->num_UV * 3;

	char* buffer = new char[bufferSize]; // Creating the Buffer
	char* it = buffer;
	std::memcpy(buffer, bufferIdx, sizeof(bufferIdx)); // Allocating Indexes
	it += sizeof(bufferIdx);
	std::memcpy(it, m->vertices, sizeof(float) * m->num_vertices * 3); // Allocating vertices
	it += sizeof(float) * m->num_vertices * 3;
	std::memcpy(it, m->indices, sizeof(unsigned int) * m->num_indices); // Allocating indices
	if (m->num_UV > 0) {
		it += sizeof(unsigned int) * m->num_indices;
		std::memcpy(it, m->textureCoords, sizeof(float) * m->num_UV * 3); // Allocating UVs
	}

	ret = App->fs->SaveUnique(LIBRARY_MESHES, buffer, m->name.c_str(), MESH_OWN_FORMAT, bufferSize, output_file, true);

	if (buffer != nullptr)
		mdelete[] buffer;

	return ret;
}

ComponentMesh* ModuleAssimp::LoadMyFormatMesh(const char * exported_file)
{
	ComponentMesh* new_mesh = nullptr;

	if (exported_file == nullptr) {
		LOG("ERROR Empty or unvalid Path");
		return false;
	}
	const char* str = GetCExtension(exported_file);
	if (std::strcmp(GetCExtension(exported_file), MESH_OWN_FORMAT) == false) {
		LOG("ERROR not '%s' extension file", MESH_OWN_FORMAT);
		return false;
	}

	char* buffer = nullptr;
	int bufferSize = App->fs->Load(exported_file, &buffer);
	if (buffer != nullptr) {
		if (bufferSize > 0) {
			// [FORMAT] nV nI nUV - Vs Is UVs
			new_mesh = new ComponentMesh();
			memcpy(&new_mesh->num_vertices, buffer, sizeof(uint));
			memcpy(&new_mesh->num_indices, buffer + sizeof(uint), sizeof(uint));
			memcpy(&new_mesh->num_UV, buffer + 2 * sizeof(uint), sizeof(uint));
			char* it = buffer;
			// Vertices
			it = buffer + sizeof(uint) * NUM_MESH_IDX; // it = buffer[Vs]
			new_mesh->vertices = new float[new_mesh->num_vertices * 3];
			memcpy(new_mesh->vertices, it, sizeof(float) * new_mesh->num_vertices * 3);
			glGenBuffers(1, (GLuint*)&(new_mesh->id_vertices));
			glBindBuffer(GL_ARRAY_BUFFER, new_mesh->id_vertices);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * new_mesh->num_vertices * 3, new_mesh->vertices, GL_STATIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			// Indices
			it += sizeof(float) * new_mesh->num_vertices * 3; // it = buffer[Is]
			new_mesh->indices = new uint[new_mesh->num_indices];
			memcpy(new_mesh->indices, it, sizeof(uint) * new_mesh->num_indices);
			glGenBuffers(1, (GLuint*) &(new_mesh->id_indices));
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, new_mesh->id_indices);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * new_mesh->num_indices, new_mesh->indices, GL_STATIC_DRAW);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			// UVs
			if (new_mesh->num_UV > 0) {
				it += sizeof(uint) * new_mesh->num_indices; // it = buffer[UVs]
				new_mesh->textureCoords = new float[new_mesh->num_UV * 3];
				memcpy(new_mesh->textureCoords, it, sizeof(float) * new_mesh->num_UV * 3);

				glGenBuffers(1, (GLuint*)&(new_mesh->id_UV));
				glBindBuffer(GL_ARRAY_BUFFER, (GLuint)new_mesh->id_UV);
				glBufferData(GL_ARRAY_BUFFER, sizeof(uint) * new_mesh->num_UV * 3, new_mesh->textureCoords, GL_STATIC_DRAW);
				glBindBuffer(GL_ARRAY_BUFFER, 0);
			}
			else
				LOG("Mesh with no Uvs");
			// Name
			new_mesh->name = GetFileFromPath(exported_file, false);

			if (new_mesh != nullptr) {
				LOG("Loaded mesh with %d vertices %d indices %d UVs", new_mesh->num_vertices, new_mesh->num_indices, new_mesh->num_UV);
			}
		}
		mdelete[] buffer;
	}

	return new_mesh;
}

ComponentMesh * ModuleAssimp::LoadToOwnFormat(const char * path, const uint pprocess_flag)
{
	ComponentMesh* m = nullptr;
	
	App->editor->ClearLog();
	
	const aiScene* scene = aiImportFile(path, pprocess_flag);
	if (scene == nullptr) {
		LOG("Unvalid Path:\n\t'%s'", path);
		return nullptr;
	}
	const aiNode* node = scene->mRootNode;
	
	std::string mesh_path;
	for (uint i = 0; i < node->mNumMeshes; i++) {
		m = LoadToOwnFormat(scene->mMeshes[node->mMeshes[i]], mesh_path);
	}

	aiReleaseImport(scene);

	return m;
}

ComponentMesh * ModuleAssimp::LoadToOwnFormat(const aiMesh * m)
{
	ComponentMesh* new_m = nullptr;

	std::string output_file;
	if (Import(m, output_file)) {
		new_m = LoadMyFormatMesh(output_file.c_str());
		if (new_m == nullptr)
			LOG("ERROR Loading Mesh from '%s' format", MESH_OWN_FORMAT);
	}
	else {
		LOG("ERROR Importing Mesh to '%s' format", MESH_OWN_FORMAT);
	}

	if (new_m != nullptr) {
		//new_m->path = output_file.c_str();
		App->res->meshes.push_back(new_m);
	}

	return new_m;
}

ComponentMesh * ModuleAssimp::LoadToOwnFormat(const aiMesh * m, std::string& output_file)
{
	ComponentMesh* new_m = nullptr;

	if (Import(m, output_file)) {
		new_m = LoadMyFormatMesh(output_file.c_str());
		if (new_m == nullptr)
			LOG("ERROR Loading Mesh from '%s' format", MESH_OWN_FORMAT);
	}
	else {
		LOG("ERROR Importing Mesh to '%s' format", MESH_OWN_FORMAT);
	}

	if (new_m != nullptr) {
		//new_m->path = output_file.c_str();
		App->res->meshes.push_back(new_m);
	}

	return new_m;
}

ComponentMesh * ModuleAssimp::LoadMyMeshFile(const char * buffer, const char* filename)
{
	ComponentMesh* cmesh = nullptr;

	std::string out;

	if(App->fs->SaveUnique(LIBRARY_MESHES, buffer, filename, MESH_OWN_FORMAT, strlen(buffer) * sizeof(char), out, false))
		cmesh = LoadMyFormatMesh(out.c_str());

	return cmesh;
}

bool ModuleAssimp::CleanUp()
{
	return true;
}