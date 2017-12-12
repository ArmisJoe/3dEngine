#include "ComponentMesh.h"

#include "Application.h"

#include "ImGui\imgui.h"
#include <string>

#include "ComponentTransform.h"

#include "md5.h"

ComponentMesh::ComponentMesh() : Component(componentType_Mesh)
{
	typeName = "Mesh";
	name = "Mesh";
	indices = nullptr;
	vertices = nullptr;
	textureCoords = nullptr;
}

ComponentMesh::ComponentMesh(GameObject* argparent) : Component(componentType_Mesh, argparent)
{
	typeName = "Mesh";
	name = "Mesh";
	indices = nullptr;
	vertices = nullptr;
	textureCoords = nullptr;
}

ComponentMesh::ComponentMesh(componentType argtype, GameObject * argparent) : Component(componentType_Mesh, argparent)
{
	typeName = "Mesh";
	name = "Mesh";
	indices = nullptr;
	vertices = nullptr;
	textureCoords = nullptr;
}

void ComponentMesh::Start()
{
	GetParent()->CreateAABBFromMesh(this);
}

void ComponentMesh::Update(float dt)
{
}

void ComponentMesh::CleanUp()
{
	if (vertices != nullptr)
		mdelete[] vertices;
	if (indices != nullptr)
		mdelete[] indices;
	if (textureCoords != nullptr)
		mdelete[] textureCoords;
	/*if (normals != nullptr)
		mdelete[] normals;*/
	if (skin != nullptr)
		mdelete skin;

	indices = nullptr;
	vertices = nullptr;
	textureCoords = nullptr;

}

void ComponentMesh::Draw(const ComponentTransform* trans, const ComponentMaterial * mat)
{
}

void ComponentMesh::OnEditor()
{
	ImGui::TextColored(COLOR_YELLOW, "Mesh:");
		std::string str = "Triangles: " + std::to_string(this->GetTriangles());
		ImGui::Text(str.c_str());
		str = "Vertices: " + std::to_string(this->num_vertices);
		ImGui::Text(str.c_str());
		ImGui::SameLine();
		str = "Indices: " + std::to_string(this->num_indices);
		ImGui::Text(str.c_str());
}
void ComponentMesh::Serialize(JSON_Doc * doc)
{
	if (doc == NULL)
		return;

	char* buffer;
	App->fs->Load(path.c_str(), &buffer);

	std::string meshdoc = md5(buffer).c_str();

	doc->SetNumber("type", type);
	doc->SetNumber("parentUID", (parent != nullptr) ? parent->GetUID() : -1);
	doc->SetString("path", path.c_str());
	doc->SetString("rawpath", raw_path.c_str());
	doc->SetString("meshdoc", meshdoc.c_str());

	if (buffer != nullptr)
		mdelete[] buffer;
}

// ------------------------- ANIMATION -------------------------

void ComponentMesh::BindSkin()
{
	
	// Bind Skin
	// Vertices
	glGenBuffers(1, (GLuint*) &(skin->id_vertices));
	glBindBuffer(GL_ARRAY_BUFFER, skin->id_vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * skin->num_vertices * 3, skin->vertices, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Indices
	glGenBuffers(1, (GLuint*) &(skin->id_indices));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, skin->id_indices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * skin->num_indices, skin->indices, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// UVs
	if (skin->num_UV > 0) {
		glGenBuffers(1, (GLuint*)&(skin->id_UV));
		glBindBuffer(GL_ARRAY_BUFFER, (GLuint)skin->id_UV);
		glBufferData(GL_ARRAY_BUFFER, sizeof(uint) * skin->num_UV * 3, skin->textureCoords, GL_DYNAMIC_DRAW);
	}

}

void ComponentMesh::DoSkin(GameObject* go)
{
	//vector<Component*> anims = GetParent()->FindComponents(componentType_Animation);
	//I need the bones to attach them

	//after this the skin should be re-copied
	//SetSkin();
	// we send it to the gpu here again
	// i have no idea how to do that but just a reminder I guess

}

void ComponentMesh::ResetDeformableMesh()
{
	if (skin != nullptr) {
		skin->CleanUp();
		mdelete skin;
	}
	skin = new ComponentMesh();
	// fill the skin here with something like:
	// skin->FillYourself(); [DEPRECATED] -> now FeelYourself();

	skin->num_vertices = num_vertices;
	skin->num_indices = num_indices;
	skin->num_UV = num_UV;

	skin->vertices = new float[num_vertices * 3];
	skin->indices = new uint[num_indices];
	skin->textureCoords = new float[skin->num_UV * 3];

	memcpy(skin->vertices, this->vertices, sizeof(float) * num_vertices * 3);
	memcpy(skin->indices, this->indices, sizeof(uint) * num_indices);
	memcpy(skin->textureCoords, this->textureCoords, sizeof(float) * num_UV * 3);
	//normals here

	// Name
	skin->name = name;

	// Parent -- Since it is a tmp mesh, we don't need to put it on the component list of the parent. 
	//    So links go like this -- skin -> parent -x> skin
	skin->parent = this->GetParent();

}

