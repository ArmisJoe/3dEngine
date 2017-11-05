#include "ComponentMesh.h"

#include "Application.h"

#include "ImGui\imgui.h"
#include <string>

#include "ComponentTransform.h"

ComponentMesh::ComponentMesh() : Component(componentType_Mesh)
{
	name = "Mesh";
}

ComponentMesh::ComponentMesh(GameObject* argparent) : Component(componentType_Mesh, argparent)
{
	name = "Mesh";
}

ComponentMesh::ComponentMesh(componentType argtype, GameObject * argparent) : Component(componentType_Mesh, argparent)
{
	name = "Mesh";
}

void ComponentMesh::Start()
{
	GetParent()->CreateAABBFromMesh(this);
}

void ComponentMesh::Update(float dt)
{
	if (visible) {
		ComponentMaterial* mat = nullptr;
		if (!this->GetParent()->FindComponents(componentType_Material).empty()) {
			mat = (ComponentMaterial*)this->GetParent()->FindComponents(componentType_Material)[0];
		}
		ComponentTransform* trans = nullptr;
		if (!this->GetParent()->FindComponents(componentType_Transform).empty()) {
			trans = (ComponentTransform*)this->GetParent()->FindComponents(componentType_Transform)[0];
		}
		Draw(trans, mat);
	}
}

void ComponentMesh::Draw(const ComponentTransform* trans, const ComponentMaterial * mat)
{
	glEnableClientState(GL_VERTEX_ARRAY);

	if (trans != nullptr) {
		glPushMatrix();
		glMultMatrixf(trans->GetMatrix4x4());
	}

	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, id_vertices);

	glVertexPointer(3, GL_FLOAT, 0, NULL);

	// Applying Material
	if (mat != nullptr) {
		//Apply UV if exist
		if (num_UV != 0)
		{
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glBindBuffer(GL_ARRAY_BUFFER, id_UV);
			glTexCoordPointer(3, GL_FLOAT, 0, NULL);
		}

		//Diffuse Channel
		if (mat->GetTextureChannel(texType_Diffuse) != nullptr)
			glBindTexture(GL_TEXTURE_2D, (GLuint)mat->GetTextureChannel(texType_Diffuse)->id);

	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_indices);
	glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_INT, NULL);


	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	if(trans != nullptr)
		glPopMatrix();
}

void ComponentMesh::OnEditor()
{
	std::string str = "Triangles: " + std::to_string(this->num_triangles);
	ImGui::Text(str.c_str());
	str = "Vertices: " + std::to_string(this->num_vertices);
	ImGui::Text(str.c_str());
	ImGui::SameLine();
	str = "Indices: " + std::to_string(this->num_indices);
	ImGui::Text(str.c_str());
};
