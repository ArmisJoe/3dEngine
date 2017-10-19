#include "ModuleAssimp.h"
#include "Application.h"
#include "ModuleCamera3D.h"


ModuleAssimp::ModuleAssimp(Application * app, bool start_enabled) : Module(app, start_enabled)
{
	name = "Assimp";
}

ModuleAssimp::~ModuleAssimp()
{
}

std::vector<ComponentMesh*> ModuleAssimp::LoadGeometry(const char* path, const unsigned int pprocess_flag)
{

	App->editor->ClearLog();

	//struct aiLogStream stream;
	//stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	//aiAttachLogStream(&stream);

	std::vector<ComponentMesh*> ms;

	const aiScene* scene = aiImportFile(path, pprocess_flag);
	aiMaterial** ai_mat = nullptr;
	if (scene != nullptr && scene->HasMeshes())
	{
		//Scene Materials
		if (scene->HasMaterials()) {
			ai_mat = scene->mMaterials;
		}
		else {
			LOG("Scene with No Materials");
		}
		// Scene Meshes
		for (int i = 0; i < scene->mNumMeshes; i++) {
			//Vertices
			ComponentMesh* new_mesh = nullptr;
			aiMesh* m = scene->mMeshes[i];
			new_mesh = new ComponentMesh();
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

			//Material
			if (ai_mat != nullptr) {
				aiMaterial* m_mat = nullptr;
				m_mat = ai_mat[(int)new_mesh->material_index];
				Material* mat = nullptr;
				mat = new Material();
				// Diffuse
				if (m_mat->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
					for (uint i = 0; i < m_mat->GetTextureCount(aiTextureType_DIFFUSE); i++) {
						aiString m_path;
						m_mat->GetTexture(aiTextureType_DIFFUSE, i, &m_path);
						if(m_path.length > 0)
							mat->diffuse.push_back(App->tex->LoadTexture(m_path.C_Str()));
					}
				}
				else {
					LOG("No Diffuse found")
				}
			}

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
			LOG("New mesh with %d vertices %d indices %d UVs", new_mesh->num_vertices, new_mesh->num_indices, new_mesh->num_UV);
			meshes.push_back(new_mesh);
			ms.push_back(new_mesh);
			// camera Focus
			App->camera->FocusMesh(new_mesh->vertices, new_mesh->num_vertices);
		}

		aiReleaseImport(scene);
	}
	else
		LOG("Error loading scene %s", path);

	return ms;

}

void ModuleAssimp::GenerateVerticesBuffer(const Mesh & mesh)
{
	glGenBuffers(1, (GLuint*) &(mesh.id_vertices));
	glBindBuffer(GL_ARRAY_BUFFER, mesh.id_vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*mesh.num_vertices * 3, mesh.vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void ModuleAssimp::GenerateIndicesBuffer(const Mesh & mesh)
{
	glGenBuffers(1, (GLuint*)&(mesh.id_indices));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.id_indices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * mesh.num_indices, mesh.indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

bool ModuleAssimp::CleanUp()
{

	while (!meshes.empty())
	{
		if(meshes.front() != nullptr)
			delete[] meshes.front();
		meshes.pop_front();
	}

	//aiDetachAllLogStreams();
	return true;
}