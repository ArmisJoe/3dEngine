#include "ModuleAssimp.h"
#include "Application.h"

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
			aiMesh* new_mesh = scene->mMeshes[i];
			Mesh m;

			m.num_vertices = new_mesh->mNumVertices;
			m.vertices = new float[m.num_vertices * 3];
			memcpy(m.vertices, new_mesh->mVertices, sizeof(float) * m.num_vertices * 3);

			log_string = "New mesh with " + m.num_vertices;
			log_string.append(" vertices");
			App->imgui->ConsoleLog(log_string.c_str());

			if (new_mesh->HasFaces()) {
				m.num_indices = new_mesh->mNumFaces * 3;
				m.indices = new uint[m.num_indices]; // triangles
				for (uint i = 0; i < new_mesh->mNumFaces; i++) {
					if (new_mesh->mFaces[i].mNumIndices != 3)
						App->imgui->ConsoleLog("WARNING: Geometry face with != 3 indices");
					else
						memcpy(&m.indices[i * 3], new_mesh->mFaces[i].mIndices, 3 * sizeof(uint));

				}
			}
			GenerateMeshBuffer(m);
			meshes.push_back(&m);
		}


		// !_Mesh Iteration

		aiReleaseImport(scene);
	}
	else {
		log_string = "Error loading scene: ";
		log_string.append(path);
		App->imgui->ConsoleLog(log_string.c_str());
	}


}

void ModuleAssimp::GenerateMeshBuffer(const Mesh & mesh)
{
	glGenBuffers(1, (GLuint*) &(mesh.id_vertices));
	glBindBuffer(GL_ARRAY_BUFFER, mesh.id_vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*mesh.num_vertices * 3, mesh.vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, (GLuint*)&(mesh.id_indices));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.id_indices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * mesh.num_indices, mesh.indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

bool ModuleAssimp::CleanUp()
{
	aiDetachAllLogStreams();
	return true;
}
