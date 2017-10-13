#include "ModuleAssimp.h"
#include "Application.h"

ModuleAssimp::ModuleAssimp(Application * app, bool start_enabled) : Module(app, start_enabled)
{
	name = "Assimp";
}

ModuleAssimp::~ModuleAssimp()
{
}

void ModuleAssimp::LoadGeometry(const char* path, const unsigned int pprocess_flag)
{
	std::string log_string;
	aiLogStream log_stream;
	log_stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&log_stream);


	const aiScene* scene = aiImportFile(path, pprocess_flag);

	if (scene != nullptr && scene->HasMeshes()) {
		
		// Mesh Iteration
		for (int i = 0; i < scene->mNumMeshes; i++) {
			//Vertices
			aiMesh* m = scene->mMeshes[i];
			Mesh* new_mesh = new Mesh();
			// Texture UV's 'load'
			new_mesh->num_UVChannels = m->GetNumUVChannels();
			memcpy(new_mesh->textureCoords, m->mTextureCoords, sizeof(float)*new_mesh->num_vertices * 3);
			// Material Index
			new_mesh->material_index = m->mMaterialIndex;
			// Vertices
			new_mesh->num_vertices = m->mNumVertices;
			new_mesh->vertices = new float[new_mesh->num_vertices * 3];
			memcpy(new_mesh->vertices, m->mVertices, sizeof(float) * new_mesh->num_vertices * 3);
			LOG("New mesh with %d vertices", new_mesh->num_vertices);
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
						LOG("WARNING, geometry face with != 3 indices!");
					}
					else
						memcpy(&new_mesh->indices[i * 3], m->mFaces[i].mIndices, 3 * sizeof(uint));
				}

			}
			glGenBuffers(1, (GLuint*) &(new_mesh->id_indices));
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, new_mesh->id_indices);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * new_mesh->num_indices, new_mesh->indices, GL_STATIC_DRAW);

			meshes.push_back(new_mesh);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		}
		// !_Mesh Iteration

		aiReleaseImport(scene);
	}
	else {
		LOG("Error Loading Scene %s", path);
	}


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
		delete[] meshes.front();
		meshes.pop_front();
	}

	aiDetachAllLogStreams();
	return true;
}
